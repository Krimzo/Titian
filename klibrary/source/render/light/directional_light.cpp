#include "klibrary.h"


kl::DirectionalLight::DirectionalLight(GPU& gpu, const int resolution)
    : m_gpu(gpu)
{
    set_resolution(resolution);
}

void kl::DirectionalLight::set_resolution(const int resolution)
{
    m_resolution = resolution;

    dx::TextureDescriptor shadow_map_descriptor{};
    shadow_map_descriptor.Width = resolution;
    shadow_map_descriptor.Height = resolution;
    shadow_map_descriptor.MipLevels = 1;
    shadow_map_descriptor.ArraySize = 1;
    shadow_map_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
    shadow_map_descriptor.SampleDesc.Count = 1;
    shadow_map_descriptor.Usage = D3D11_USAGE_DEFAULT;
    shadow_map_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    dx::DepthViewDescriptor shadow_depth_view_descriptor{};
    shadow_depth_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
    shadow_depth_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    dx::ShaderViewDescriptor shadow_shader_view_descriptor{};
    shadow_shader_view_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
    shadow_shader_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shadow_shader_view_descriptor.Texture2D.MipLevels = 1;

    for (auto& cascade : m_cascades) {
        cascade = new Texture(m_gpu);
        cascade->texture = m_gpu.create_texture(&shadow_map_descriptor, nullptr);
        cascade->create_depth_view(&shadow_depth_view_descriptor);
        cascade->create_shader_view(&shadow_shader_view_descriptor);
    }
}

int kl::DirectionalLight::resolution() const
{
    return m_resolution;
}

void kl::DirectionalLight::set_direction(const Float3& direction)
{
    m_direction = normalize(direction);
}

kl::Float3 kl::DirectionalLight::direction() const
{
    return m_direction;
}

kl::dx::DepthView kl::DirectionalLight::depth_view(const UINT cascade_index) const
{
    return m_cascades[cascade_index]->depth_view;
}

kl::dx::ShaderView kl::DirectionalLight::shader_view(const UINT cascade_index) const
{
    return m_cascades[cascade_index]->shader_view;
}

kl::Float4x4 kl::DirectionalLight::matrix(const Float4x4& inv_cam_mat) const
{
    Float4 frustum_corners[8] = {
        inv_cam_mat * Float4(-1, -1, -1, 1),
        inv_cam_mat * Float4( 1, -1, -1, 1),
        inv_cam_mat * Float4(-1,  1, -1, 1),
        inv_cam_mat * Float4( 1,  1, -1, 1),
        inv_cam_mat * Float4(-1, -1,  1, 1),
        inv_cam_mat * Float4( 1, -1,  1, 1),
        inv_cam_mat * Float4(-1,  1,  1, 1),
        inv_cam_mat * Float4( 1,  1,  1, 1),
    };
    for (auto& corner : frustum_corners) {
        corner *= (1.0f / corner.w);
    }

    const Float4x4 temp_light_view_matrix = Float4x4::look_at({}, m_direction, { 0, 1, 0 });
    for (auto& corner : frustum_corners) {
        corner = temp_light_view_matrix * corner;
    }

    Float2 min_xy{ std::numeric_limits<float>::infinity() };
    Float2 max_xy{ -std::numeric_limits<float>::infinity() };
    float min_z = std::numeric_limits<float>::infinity();
    for (const auto& corner : frustum_corners) {
        min_xy.x = min(min_xy.x, corner.x);
        min_xy.y = min(min_xy.y, corner.y);

        max_xy.x = max(max_xy.x, corner.x);
        max_xy.y = max(max_xy.y, corner.y);

        min_z = min(min_z, corner.z);
    }
    Float3 light_position{
        (min_xy.x + max_xy.x) * 0.5f,
        (min_xy.y + max_xy.y) * 0.5f,
        min_z,
    };

    const Float4x4 temp_light_view_matrix_inverse = inverse(temp_light_view_matrix);
    const Float4 new_light_pos = temp_light_view_matrix_inverse * Float4(light_position.x, light_position.y, light_position.z, 1.0f);
    light_position = new_light_pos.xyz();
    for (auto& corner : frustum_corners) {
        corner = temp_light_view_matrix_inverse * corner;
    }

    const Float4x4 light_view_matrix = Float4x4::look_at(light_position, light_position + m_direction, { 0, 1, 0 });
    for (auto& corner : frustum_corners) {
        corner = light_view_matrix * corner;
    }

    Float3 max_xyz{ -std::numeric_limits<float>::infinity() };
    for (const auto& corner : frustum_corners) {
        max_xyz.x = max(max_xyz.x, corner.x);
        max_xyz.y = max(max_xyz.y, corner.y);
        max_xyz.z = max(max_xyz.z, corner.z);
    }

    const Float4x4 light_projection_matrix = Float4x4::orthographic(
        -max_xyz.x, max_xyz.x,
        -max_xyz.x, max_xyz.x,
        -max_xyz.z, max_xyz.z
    );
    return light_projection_matrix * light_view_matrix;
}

kl::Float4x4 kl::DirectionalLight::matrix(Camera camera, const UINT cascade_index) const
{
    const Float2 camera_planes = { camera.near_plane, camera.far_plane };
    camera.near_plane = lerp(cascades[cascade_index + 0], camera_planes.x, camera_planes.y);
    camera.far_plane = lerp(cascades[cascade_index + 1], camera_planes.x, camera_planes.y);
    return matrix(inverse(camera.matrix()));
}
