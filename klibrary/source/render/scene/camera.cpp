#include "klibrary.h"


void kl::Camera::update_aspect_ratio(const Int2 size)
{
    aspect_ratio = size.x / (float) size.y;
}

kl::Ray kl::Camera::ray() const
{
    return { position, m_forward };
}

void kl::Camera::set_forward(const Float3& dir)
{
    m_forward = normalize(dir);
}

void kl::Camera::set_up(const Float3& dir)
{
    m_up = normalize(dir);
}

kl::Float3 kl::Camera::forward() const
{
    return m_forward;
}

kl::Float3 kl::Camera::right() const
{
    return cross(m_up, m_forward);
}

kl::Float3 kl::Camera::up() const
{
    return m_up;
}

void kl::Camera::move_forward(const float delta_time)
{
    position += m_forward * (speed * delta_time);
}

void kl::Camera::move_back(const float delta_time)
{
    position -= m_forward * (speed * delta_time);
}

void kl::Camera::move_right(const float delta_time)
{
    position += right() * (speed * delta_time);
}

void kl::Camera::move_left(const float delta_time)
{
    position -= right() * (speed * delta_time);
}

void kl::Camera::move_up(const float delta_time)
{
    position += m_up * (speed * delta_time);
}

void kl::Camera::move_down(const float delta_time)
{
    position -= m_up * (speed * delta_time);
}

void kl::Camera::rotate(const Float2 mouse_pos, const Float2 frame_center, const float vertical_angle_limit)
{
    const Float2 rotation = (mouse_pos - frame_center) * sensitivity;
    const Float3 forward_vert = kl::rotate(m_forward, right(), rotation.y);
    if (abs(angle(forward_vert, m_up) - 90.0f) <= vertical_angle_limit) {
        set_forward(forward_vert);
    }
    set_forward(kl::rotate(m_forward, m_up, rotation.x));
}

kl::Float4x4 kl::Camera::view_matrix() const
{
    return Float4x4::look_at(position, position + m_forward, m_up);
}

kl::Float4x4 kl::Camera::projection_matrix() const
{
    return Float4x4::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
}

kl::Float4x4 kl::Camera::matrix() const
{
    return projection_matrix() * view_matrix();
}
