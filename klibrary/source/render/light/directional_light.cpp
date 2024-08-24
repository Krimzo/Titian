#include "klibrary.h"


kl::DirectionalLight::DirectionalLight(GPU* gpu, const UINT map_resolution)
    : map_resolution(map_resolution)
{
    dx::TextureDescriptor shadow_map_descriptor = {};
    shadow_map_descriptor.Width = map_resolution;
    shadow_map_descriptor.Height = map_resolution;
    shadow_map_descriptor.MipLevels = 1;
    shadow_map_descriptor.ArraySize = 1;
    shadow_map_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
    shadow_map_descriptor.SampleDesc.Count = 1;
    shadow_map_descriptor.Usage = D3D11_USAGE_DEFAULT;
    shadow_map_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    dx::DepthViewDescriptor shadow_depth_view_descriptor = {};
    shadow_depth_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
    shadow_depth_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    dx::ShaderViewDescriptor shadow_shader_view_descriptor = {};
    shadow_shader_view_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
    shadow_shader_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shadow_shader_view_descriptor.Texture2D.MipLevels = 1;

    for (auto& cascade : m_cascades) {
        cascade = new Texture(gpu);
        cascade->graphics_buffer = gpu->create_texture(&shadow_map_descriptor, nullptr);
        cascade->create_depth_view(&shadow_depth_view_descriptor);
        cascade->create_shader_view(&shadow_shader_view_descriptor);
    }
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

kl::Float4x4 kl::DirectionalLight::matrix(Camera camera, const UINT cascade_index) const
{
    const Float2 old_camera_planes = { camera.near_plane, camera.far_plane };
    camera.near_plane = lerp(CASCADE_SPLITS[cascade_index + 0], old_camera_planes.x, old_camera_planes.y);
    camera.far_plane = lerp(CASCADE_SPLITS[cascade_index + 1], old_camera_planes.x, old_camera_planes.y);
    const Float4x4 inverse_camera_matrix = inverse(camera.matrix());

    // Calculate 8 corners in world-space
    Float4 frustum_corners[8] = {
        inverse_camera_matrix * Float4(-1, -1, -1, 1),
        inverse_camera_matrix * Float4( 1, -1, -1, 1),
        inverse_camera_matrix * Float4(-1,  1, -1, 1),
        inverse_camera_matrix * Float4( 1,  1, -1, 1),

        inverse_camera_matrix * Float4(-1, -1,  1, 1),
        inverse_camera_matrix * Float4( 1, -1,  1, 1),
        inverse_camera_matrix * Float4(-1,  1,  1, 1),
        inverse_camera_matrix * Float4( 1,  1,  1, 1),
    };

    for (auto& corner : frustum_corners) {
        corner *= (1.0f / corner.w);
    }

    // Convert corners to temp light-view-space
    const Float4x4 temp_light_view_matrix = Float4x4::look_at({}, m_direction, { 0, 1, 0 });
    for (auto& corner : frustum_corners) {
        corner = temp_light_view_matrix * corner;
    }

    // Find min-max x and y in light-space
    Float2 min_xy = { std::numeric_limits<float>::infinity(),  std::numeric_limits<float>::infinity() };
    Float2 max_xy = { -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
    float min_z = std::numeric_limits<float>::infinity();
    for (const auto& corner : frustum_corners) {
        min_xy.x = min(min_xy.x, corner.x);
        min_xy.y = min(min_xy.y, corner.y);

        max_xy.x = max(max_xy.x, corner.x);
        max_xy.y = max(max_xy.y, corner.y);

        min_z = min(min_z, corner.z);
    }

    // Find center of near plane in light-space
    Float3 light_position = {
        (min_xy.x + max_xy.x) * 0.5f,
        (min_xy.y + max_xy.y) * 0.5f,
        min_z
    };

    // Convert temp light-space to world-space
    const Float4x4 temp_light_view_matrix_inverse = inverse(temp_light_view_matrix);
    const Float4 new_light_pos = temp_light_view_matrix_inverse * Float4(light_position.x, light_position.y, light_position.z, 1.0f);
    light_position = { new_light_pos.x, new_light_pos.y, new_light_pos.z };
    for (auto& corner : frustum_corners) {
        corner = temp_light_view_matrix_inverse * corner;
    }

    // Convert corners to proper light-view-space
    const Float4x4 light_view_matrix = Float4x4::look_at(light_position, light_position + m_direction, { 0, 1, 0 });
    for (auto& corner : frustum_corners) {
        corner = light_view_matrix * corner;
    }

    // Find proper coordinates of frustum in light-space
    Float3 max_xyz { -std::numeric_limits<float>::infinity() };
    for (const auto& corner : frustum_corners) {
        max_xyz.x = max(max_xyz.x, corner.x);
        max_xyz.y = max(max_xyz.y, corner.y);
        max_xyz.z = max(max_xyz.z, corner.z);
    }

    // Calculate final orthographic projection
    const Float4x4 light_projection_matrix = Float4x4::orthographic(
        -max_xyz.x, max_xyz.x,
        -max_xyz.x, max_xyz.x,
        -max_xyz.z, max_xyz.z
    );

    return light_projection_matrix * light_view_matrix;
}
