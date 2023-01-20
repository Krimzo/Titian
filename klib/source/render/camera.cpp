#include "render/camera.h"


void kl::camera::set_forward(const float3& dir)
{
    forward_ = dir.normalize();
}

void kl::camera::set_up(const float3& dir)
{
    up_ = dir.normalize();
}

kl::float3 kl::camera::get_forward() const
{
    return forward_;
}

kl::float3 kl::camera::get_right() const
{
    return up_.cross(forward_);
}

kl::float3 kl::camera::get_up() const
{
    return up_;
}

void kl::camera::move_forward(const float delta_time)
{
    position = position + forward_ * (speed * delta_time);
}

void kl::camera::move_back(const float delta_time)
{
    position = position - forward_ * (speed * delta_time);
}

void kl::camera::move_right(const float delta_time)
{
    position = position + get_right() * (speed * delta_time);
}

void kl::camera::move_left(const float delta_time)
{
    position = position - get_right() * (speed * delta_time);
}

void kl::camera::move_up(const float delta_time)
{
    position = position + up_ * (speed * delta_time);
}

void kl::camera::move_down(const float delta_time)
{
    position = position - up_ * (speed * delta_time);
}

void kl::camera::rotate(const float2& mouse_pos, const float2& frame_center, const float vertical_angle_limit)
{
    const float2 rotation = (mouse_pos - frame_center) * sensitivity;

    if (const float3 forward_vert = forward_.rotate(rotation.y, get_right()); std::abs(forward_vert.angle(up_) - 90.0f) <= vertical_angle_limit) {
        set_forward(forward_vert);
    }

    set_forward(forward_.rotate(rotation.x, up_));
}

kl::mat4 kl::camera::matrix() const
{
    const mat4 view = mat4::look_at(position, position + forward_, up_);
    const mat4 proj = mat4::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
    return proj * view;
}
