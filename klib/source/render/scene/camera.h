#pragma once

#include "render/components/texture.h"
#include "memory/memory.h"
#include "math/math.h"


namespace kl {
    class camera
    {
        float3 forward_ = { 0.0f, 0.0f, 1.0f };
        float3 up_ = { 0.0f, 1.0f, 0.0f };

    public:
        float3 origin = {};

        float aspect_ratio = 1.7778f;
        float field_of_view = 75.0f;

        float near_plane = 0.01f;
        float far_plane = 500.0f;

        float sensitivity = 0.1f;
        float speed = 2.0f;

        color background = {};
        ref<texture> skybox = nullptr;

        void update_aspect_ratio(const int2& size);

        void set_forward(const float3& dir);
        void set_up(const float3& dir);

        float3 get_forward() const;
        float3 get_right() const;
        float3 get_up() const;

        void move_forward(float delta_time);
        void move_back(float delta_time);
        void move_right(float delta_time);
        void move_left(float delta_time);
        void move_up(float delta_time);
        void move_down(float delta_time);

        void rotate(const float2& mouse_pos, const float2& frame_center, float vertical_angle_limit = 85.0f);

        float4x4 view_matrix() const;
        float4x4 projection_matrix() const;
        float4x4 matrix() const;
    };
}
