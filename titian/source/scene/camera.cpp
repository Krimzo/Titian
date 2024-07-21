#include "titian.h"


titian::Camera::Camera(px::PxPhysics* physics, const bool dynamic)
    : Entity(EntityType::CAMERA, physics, dynamic)
{}

void titian::Camera::serialize(Serializer* serializer, const void* helper_data) const
{
    Entity::serialize(serializer, helper_data);

    serializer->write_object<int>(type);

    serializer->write_object<float>(aspect_ratio);
    serializer->write_object<float>(field_of_view);
    serializer->write_object<float>(width);
    serializer->write_object<float>(height);
    serializer->write_object<float>(near_plane);
    serializer->write_object<float>(far_plane);
    serializer->write_object<float>(sensitivity);
    serializer->write_object<float>(speed);

    serializer->write_object<Float3>(m_forward);
    serializer->write_object<Float3>(m_up);

    serializer->write_object<Color>(background);
    serializer->write_object<Float4x4>(custom_data);

    serializer->write_string(skybox_name);
    serializer->write_string(shader_name);
}

void titian::Camera::deserialize(const Serializer* serializer, const void* helper_data)
{
    Entity::deserialize(serializer, helper_data);

    serializer->read_object<int>(type);

    serializer->read_object<float>(aspect_ratio);
    serializer->read_object<float>(field_of_view);
    serializer->read_object<float>(width);
    serializer->read_object<float>(height);
    serializer->read_object<float>(near_plane);
    serializer->read_object<float>(far_plane);
    serializer->read_object<float>(sensitivity);
    serializer->read_object<float>(speed);

    serializer->read_object<Float3>(m_forward);
    serializer->read_object<Float3>(m_up);

    serializer->read_object<Color>(background);
    serializer->read_object<Float4x4>(custom_data);

    serializer->read_string(skybox_name);
    serializer->read_string(shader_name);
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
    if (std::abs(angle(forward_vert, m_up) - 90.0f) <= vertical_angle_limit) {
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
    if (type == CameraType::ORTHOGRAPHIC) {
        return Float4x4::orthographic(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, near_plane, far_plane);
    }
    return Float4x4::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
}

titian::Float4x4 titian::Camera::camera_matrix() const
{
    return projection_matrix() * view_matrix();
}
