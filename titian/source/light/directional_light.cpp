#include "titian.h"


titian::DirectionalLight::DirectionalLight()
{
    set_resolution(2500);
}

void titian::DirectionalLight::serialize(Serializer& serializer) const
{
    Entity::serialize(serializer);

    serializer.write_float_array("color", &color.x, 3);
    serializer.write_float("point_size", point_size);
    serializer.write_float_array("cascade_splits", cascade_splits, (int) std::size(cascade_splits));

    serializer.write_int("resolution", m_resolution);
    serializer.write_float_array("direction", &m_direction.x, 3);
}

void titian::DirectionalLight::deserialize(const Serializer& serializer)
{
    Entity::deserialize(serializer);

    serializer.read_float_array("color", &color.x, 3);
    serializer.read_float("point_size", point_size);
    serializer.read_float_array("cascade_splits", cascade_splits, (int) std::size(cascade_splits));

    serializer.read_int("resolution", m_resolution);
    serializer.read_float_array("direction", &m_direction.x, 3);

    set_resolution(m_resolution);
}

void titian::DirectionalLight::set_resolution(const int resolution)
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

    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
    for (auto& cascade : m_cascades) {
        cascade = new Texture();
        cascade->graphics_buffer = gpu.create_texture(&shadow_map_descriptor, nullptr);
        cascade->create_depth_view(&shadow_depth_view_descriptor);
        cascade->create_shader_view(&shadow_shader_view_descriptor);
    }
}

int titian::DirectionalLight::resolution() const
{
    return m_resolution;
}

void titian::DirectionalLight::set_direction(const Float3& direction)
{
    m_direction = kl::normalize(direction);
}

titian::Float3 titian::DirectionalLight::direction() const
{
    return m_direction;
}

dx::DepthView titian::DirectionalLight::depth_view(const int cascade_index) const
{
    return m_cascades[cascade_index]->depth_view;
}

dx::ShaderView titian::DirectionalLight::shader_view(const int cascade_index) const
{
    return m_cascades[cascade_index]->shader_view;
}

titian::Float4x4 titian::DirectionalLight::light_matrix(Camera* camera, const int cascade_index) const
{
    const Float2 old_camera_planes = { camera->near_plane, camera->far_plane };
    camera->near_plane = kl::lerp(cascade_splits[cascade_index + 0], old_camera_planes.x, old_camera_planes.y);
    camera->far_plane = kl::lerp(cascade_splits[cascade_index + 1], old_camera_planes.x, old_camera_planes.y);
    const Float4x4 inverse_camera_matrix = kl::inverse(camera->camera_matrix());
    camera->near_plane = old_camera_planes.x;
    camera->far_plane = old_camera_planes.y;

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

    const Float4x4 temp_light_view_matrix = Float4x4::look_at({}, m_direction, { 0, 1, 0 });
    for (auto& corner : frustum_corners) {
        corner = temp_light_view_matrix * corner;
    }

    Float2 min_xy = { std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
    Float2 max_xy = { -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
    float min_z = std::numeric_limits<float>::infinity();
    for (const auto& corner : frustum_corners) {
        min_xy.x = kl::min(min_xy.x, corner.x);
        min_xy.y = kl::min(min_xy.y, corner.y);

        max_xy.x = kl::max(max_xy.x, corner.x);
        max_xy.y = kl::max(max_xy.y, corner.y);

        min_z = kl::min(min_z, corner.z);
    }

    Float3 light_position = {
        (min_xy.x + max_xy.x) * 0.5f,
        (min_xy.y + max_xy.y) * 0.5f,
        min_z
    };

    const Float4x4 temp_light_view_matrix_inverse = inverse(temp_light_view_matrix);
    const Float4 new_light_pos = temp_light_view_matrix_inverse * Float4(light_position.x, light_position.y, light_position.z, 1.0f);
    light_position = { new_light_pos.x, new_light_pos.y, new_light_pos.z };
    for (auto& corner : frustum_corners) {
        corner = temp_light_view_matrix_inverse * corner;
    }

    const Float4x4 light_view_matrix = Float4x4::look_at(light_position, light_position + m_direction, { 0, 1, 0 });
    for (auto& corner : frustum_corners) {
        corner = light_view_matrix * corner;
    }

    Float3 max_xyz{ -std::numeric_limits<float>::infinity() };
    for (const auto& corner : frustum_corners) {
        max_xyz.x = std::max(max_xyz.x, corner.x);
        max_xyz.y = std::max(max_xyz.y, corner.y);
        max_xyz.z = std::max(max_xyz.z, corner.z);
    }

    const Float4x4 light_projection_matrix = Float4x4::orthographic(
        -max_xyz.x, max_xyz.x,
        -max_xyz.x, max_xyz.x,
        -max_xyz.z, max_xyz.z
    );
    return light_projection_matrix * light_view_matrix;
}
