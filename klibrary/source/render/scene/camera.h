#pragma once

#include "render/components/texture.h"
#include "memory/memory.h"
#include "math/math.h"


namespace kl {
    struct Camera
    {
        Float3 position;

        float aspect_ratio = 1.7778f;
        float field_of_view = 75.0f;

        float near_plane = 0.01f;
        float far_plane = 500.0f;

        float sensitivity = 0.1f;
        float speed = 2.0f;

        RGB background;
        Ref<Texture> skybox;

        void update_aspect_ratio(Int2 size);
        Ray ray() const;

        void set_forward(const Float3& dir);
        void set_up(const Float3& dir);

        Float3 forward() const;
        Float3 right() const;
        Float3 up() const;

        void move_forward(float delta_time);
        void move_back(float delta_time);
        void move_right(float delta_time);
        void move_left(float delta_time);
        void move_up(float delta_time);
        void move_down(float delta_time);

        void rotate(Float2 mouse_pos, Float2 frame_center, float vertical_angle_limit = 85.0f);

        Float4x4 view_matrix() const;
        Float4x4 projection_matrix() const;
        Float4x4 matrix() const;

    private:
        Float3 m_forward = { 0.0f, 0.0f, 1.0f };
        Float3 m_up = { 0.0f, 1.0f, 0.0f };
    };
}
