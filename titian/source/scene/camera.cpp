#include "titian.h"


titian::Camera::Camera(px::PxPhysics* physics, kl::GPU* gpu)
    : Entity(physics), m_gpu(gpu)
{
    screen_texture = new Texture(gpu);
    game_color_texture = new Texture(gpu);
    game_depth_texture = new Texture(gpu);
    editor_picking_texture = new Texture(gpu);
    editor_staging_texture = new Texture(gpu);

    resize({ 1600, 900 });
    resize_staging({ 1, 1 });
}

void titian::Camera::serialize(Serializer* serializer, const void* helper_data) const
{
    Entity::serialize(serializer, helper_data);

    serializer->write_int("camera_type", camera_type);

    serializer->write_bool("enabled", enabled);
    serializer->write_bool("v_sync", v_sync);
    serializer->write_bool("render_wireframe", render_wireframe);

    serializer->write_float("aspect_ratio", aspect_ratio);
    serializer->write_float("field_of_view", field_of_view);
    serializer->write_float("width", width);
    serializer->write_float("height", height);
    serializer->write_float("near_plane", near_plane);
    serializer->write_float("far_plane", far_plane);
    serializer->write_float("sensitivity", sensitivity);
    serializer->write_float("speed", speed);

    serializer->write_float_array("forward", &m_forward.x, 3);
    serializer->write_float_array("up", &m_up.x, 3);

    serializer->write_float_array("background", &background.x, 4);
    serializer->write_float_array("custom_data", custom_data.data, 16);

    serializer->write_string("skybox_name", skybox_name);
    serializer->write_string("shader_name", shader_name);
    serializer->write_string("target_name", target_name);

    const Int2 res = resolution();
    serializer->write_int_array("resolution", &res.x, 2);
}

void titian::Camera::deserialize(const Serializer* serializer, const void* helper_data)
{
    Entity::deserialize(serializer, helper_data);

    serializer->read_int("camera_type", camera_type);

    serializer->read_bool("enabled", enabled);
    serializer->read_bool("v_sync", v_sync);
    serializer->read_bool("render_wireframe", render_wireframe);

    serializer->read_float("aspect_ratio", aspect_ratio);
    serializer->read_float("field_of_view", field_of_view);
    serializer->read_float("width", width);
    serializer->read_float("height", height);
    serializer->read_float("near_plane", near_plane);
    serializer->read_float("far_plane", far_plane);
    serializer->read_float("sensitivity", sensitivity);
    serializer->read_float("speed", speed);

    serializer->read_float_array("forward", &m_forward.x, 3);
    serializer->read_float_array("up", &m_up.x, 3);

    serializer->read_float_array("background", &background.x, 4);
    serializer->read_float_array("custom_data", custom_data.data, 16);

    serializer->read_string("skybox_name", skybox_name);
    serializer->read_string("shader_name", shader_name);
    serializer->read_string("target_name", target_name);

    Int2 res;
    serializer->read_int_array("resolution", &res.x, 2);
	resize(res);
}

void titian::Camera::update_aspect_ratio(const Int2& size)
{
    aspect_ratio = size.x / (float)size.y;
}

void titian::Camera::set_forward(const Float3& dir)
{
    m_forward = normalize(dir);
}

void titian::Camera::set_up(const Float3& dir)
{
    m_up = normalize(dir);
}

titian::Float3 titian::Camera::forward() const
{
    return m_forward;
}

titian::Float3 titian::Camera::right() const
{
    return cross(m_up, m_forward);
}

titian::Float3 titian::Camera::up() const
{
    return m_up;
}

void titian::Camera::move_forward(const float delta_time)
{
    set_position(position() + m_forward * (speed * delta_time));
}

void titian::Camera::move_back(const float delta_time)
{
    set_position(position() - m_forward * (speed * delta_time));
}

void titian::Camera::move_right(const float delta_time)
{
    set_position(position() + right() * (speed * delta_time));
}

void titian::Camera::move_left(const float delta_time)
{
    set_position(position() - right() * (speed * delta_time));
}

void titian::Camera::move_up(const float delta_time)
{
    set_position(position() + m_up * (speed * delta_time));
}

void titian::Camera::move_down(const float delta_time)
{
    set_position(position() - m_up * (speed * delta_time));
}

void titian::Camera::rotate(const Float2& mouse_pos, const Float2& frame_center, const float vertical_angle_limit)
{
    const Float2 rotation = (mouse_pos - frame_center) * sensitivity;
    const Float3 forward_vert = kl::rotate(m_forward, right(), rotation.y);
    if (kl::abs(angle(forward_vert, m_up) - 90.0f) <= vertical_angle_limit) {
        set_forward(forward_vert);
    }
    set_forward(kl::rotate(m_forward, m_up, rotation.x));
}

titian::Float4x4 titian::Camera::view_matrix() const
{
    const Float3 position = this->position();
    return Float4x4::look_at(position, position + m_forward, m_up);
}

titian::Float4x4 titian::Camera::projection_matrix() const
{
    if (camera_type == CameraType::ORTHOGRAPHIC) {
        return Float4x4::orthographic(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, near_plane, far_plane);
    }
    return Float4x4::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
}

titian::Float4x4 titian::Camera::camera_matrix() const
{
    return projection_matrix() * view_matrix();
}

bool titian::Camera::can_see(const Float3& point) const
{
    return true;
}

void titian::Camera::resize(const Int2& new_size)
{
    if (new_size.x <= 0 || new_size.y <= 0) {
        return;
    }
    if (new_size == resolution()) {
        return;
    }
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

    dx::TextureDescriptor screen_tex_descriptor{};
    screen_tex_descriptor.Width = new_size.x;
    screen_tex_descriptor.Height = new_size.y;
    screen_tex_descriptor.MipLevels = 1;
    screen_tex_descriptor.ArraySize = 1;
    screen_tex_descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    screen_tex_descriptor.SampleDesc.Count = 1;
    screen_tex_descriptor.Usage = D3D11_USAGE_DEFAULT;
    screen_tex_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    screen_texture->graphics_buffer = gpu->create_texture(&screen_tex_descriptor, nullptr);
    screen_texture->create_target_view(nullptr);
    screen_texture->create_shader_view(nullptr);

    dx::TextureDescriptor game_col_descriptor = screen_tex_descriptor;
    game_color_texture->graphics_buffer = gpu->create_texture(&game_col_descriptor, nullptr);
    game_color_texture->create_target_view(nullptr);
    game_color_texture->create_shader_view(nullptr);

    dx::TextureDescriptor  game_depth_descriptor = game_col_descriptor;
    game_depth_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
    game_depth_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    game_depth_texture->graphics_buffer = gpu->create_texture(&game_depth_descriptor, nullptr);

    dx::DepthViewDescriptor game_depth_dv_descriptor{};
    game_depth_dv_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
    game_depth_dv_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    game_depth_texture->create_depth_view(&game_depth_dv_descriptor);

    dx::ShaderViewDescriptor game_depth_sv_descriptor{};
    game_depth_sv_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
    game_depth_sv_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    game_depth_sv_descriptor.Texture2D.MipLevels = 1;
    game_depth_texture->create_shader_view(&game_depth_sv_descriptor);

    dx::TextureDescriptor editor_picking_descriptor = screen_tex_descriptor;
    editor_picking_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
    editor_picking_texture->graphics_buffer = gpu->create_texture(&editor_picking_descriptor, nullptr);
    editor_picking_texture->create_target_view(nullptr);
    editor_picking_texture->create_shader_view(nullptr);
}

void titian::Camera::resize_staging(const Int2& new_size)
{
    if (new_size.x <= 0 || new_size.y <= 0) {
        return;
    }
    if (editor_staging_texture->resolution() == new_size) {
        return;
    }
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    editor_staging_texture->graphics_buffer = gpu->create_staging_texture(editor_picking_texture->graphics_buffer, new_size);
}

titian::Int2 titian::Camera::resolution() const
{
    return screen_texture->resolution();
}

void titian::Camera::clear_targets()
{
    m_gpu->clear_target_view(game_color_texture->target_view, background);
    m_gpu->clear_depth_view(game_depth_texture->depth_view, 1.0f, 0xFF);
    m_gpu->clear_target_view(editor_picking_texture->target_view, {});
}
