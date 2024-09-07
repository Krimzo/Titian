#pragma once

#include "scene/entity.h"


namespace titian {
    enum CameraType : int32_t
    {
		PERSPECTIVE = 0,
		ORTHOGRAPHIC = 1,
    };
}

namespace titian {
    class Camera : public Entity
    {
    public:
        int32_t camera_type = CameraType::PERSPECTIVE;

        bool enabled = true;
        bool v_sync = true;
        bool render_wireframe = false;

        float field_of_view = 75.0f;
        float aspect_ratio = 1.7778f;
        float width = 16.0f;
        float height = 9.0f;
        float near_plane = 0.01f;
        float far_plane = 500.0f;
        float sensitivity = 0.1f;
        float speed = 2.0f;

        Float4 background = Color{ 30, 30, 30 };
        Float4x4 custom_data;

        String skybox_name = "/";
        String shader_name = "/";
        String target_name = "/";

        Ref<Texture> screen_texture;
        Ref<Texture> game_color_texture;
        Ref<Texture> game_depth_texture;
        Ref<Texture> editor_picking_texture;
        Ref<Texture> editor_staging_texture;

        Camera(px::PxPhysics* physics, bool dynamic, kl::GPU* gpu);

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        void update_aspect_ratio(const Int2& size);
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

        void rotate(const Float2& mouse_pos, const Float2& frame_center, float vertical_angle_limit = 85.0f);

        Float4x4 view_matrix() const;
        Float4x4 projection_matrix() const;
        Float4x4 camera_matrix() const;

        bool can_see(const Float3& point) const;

        void resize(const Int2& new_size);
        void resize_staging(const Int2& new_size);
        Int2 resolution() const;
        void clear_targets();
        
    private:
        kl::GPU* m_gpu = nullptr;
        Float3 m_forward = { 0.0f, 0.0f, 1.0f };
        Float3 m_up = { 0.0f, 1.0f, 0.0f };
    };
}
