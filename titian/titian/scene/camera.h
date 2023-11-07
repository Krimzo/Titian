#pragma once

#include "scene/entity.h"


namespace titian {
    class Camera : public Entity
    {
    public:
        float aspect_ratio = 1.7778f;
        float field_of_view = 75.0f;

        float near_plane = 0.01f;
        float far_plane = 500.0f;

        float sensitivity = 0.1f;
        float speed = 2.0f;

        kl::Color background = {};
        std::string skybox_name = "/";

        Camera(physx::PxPhysics* physics, bool dynamic);

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        void update_aspect_ratio(const kl::Int2& size);
        void set_forward(const kl::Float3& dir);
        void set_up(const kl::Float3& dir);

        kl::Float3 forward() const;
        kl::Float3 right() const;
        kl::Float3 up() const;

        void move_forward(float delta_time);
        void move_back(float delta_time);
        void move_right(float delta_time);
        void move_left(float delta_time);
        void move_up(float delta_time);
        void move_down(float delta_time);

        void rotate(const kl::Float2& mouse_pos, const kl::Float2& frame_center, float vertical_angle_limit = 85.0f);

        kl::Float4x4 view_matrix() const;
        kl::Float4x4 projection_matrix() const;
        kl::Float4x4 camera_matrix() const;
        
    private:
        kl::Float3 m_forward = { 0.0f, 0.0f, 1.0f };
        kl::Float3 m_up = { 0.0f, 1.0f, 0.0f };
    };
}
