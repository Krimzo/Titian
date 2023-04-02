#include "render/scene/camera.h"


void kl::camera::update_aspect_ratio(const int2& size)
{
    aspect_ratio = (float) size.x / size.y;
}

void kl::camera::set_forward(const float3& dir)
{
    forward_ = normalize(dir);
}

void kl::camera::set_up(const float3& dir)
{
    up_ = normalize(dir);
}

kl::float3 kl::camera::get_forward() const
{
    return forward_;
}

kl::float3 kl::camera::get_right() const
{
    return cross(up_, forward_);
}

kl::float3 kl::camera::get_up() const
{
    return up_;
}

void kl::camera::move_forward(const float delta_time)
{
    origin = origin + forward_ * (speed * delta_time);
}

void kl::camera::move_back(const float delta_time)
{
    origin = origin - forward_ * (speed * delta_time);
}

void kl::camera::move_right(const float delta_time)
{
    origin = origin + get_right() * (speed * delta_time);
}

void kl::camera::move_left(const float delta_time)
{
    origin = origin - get_right() * (speed * delta_time);
}

void kl::camera::move_up(const float delta_time)
{
    origin = origin + up_ * (speed * delta_time);
}

void kl::camera::move_down(const float delta_time)
{
    origin = origin - up_ * (speed * delta_time);
}

void kl::camera::rotate(const float2& mouse_pos, const float2& frame_center, const float vertical_angle_limit)
{
    const float2 rotation = (mouse_pos - frame_center) * sensitivity;
    const float3 forward_vert = kl::rotate(forward_, get_right(), rotation.y);

    if (::abs(angle(forward_vert, up_) - 90.0f) <= vertical_angle_limit) {
        set_forward(forward_vert);
    }

    set_forward(kl::rotate(forward_, up_, rotation.x));
}

kl::float4x4 kl::camera::view_matrix() const
{
    return float4x4::look_at(origin, origin + forward_, up_);
}

kl::float4x4 kl::camera::projection_matrix() const
{
    return float4x4::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
}

kl::float4x4 kl::camera::matrix() const
{
    return projection_matrix() * view_matrix();
}
