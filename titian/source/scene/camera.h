﻿#pragma once

#include "scene/entity.h"


namespace titian
{
enum CameraType : int32_t
{
    PERSPECTIVE = 0,
    ORTHOGRAPHIC = 1,
};
}

namespace titian
{
struct Camera : Entity
{
    CameraType camera_type = CameraType::PERSPECTIVE;

    bool enabled = true;
    bool v_sync = false;
    bool render_wireframe = false;

    float field_of_view = 75.0f;
    float aspect_ratio = 1.7778f;
    float width = 16.0f;
    float height = 9.0f;
    float near_plane = 0.01f;
    float far_plane = 500.0f;
    float sensitivity = 0.1f;
    float speed = 2.0f;

    Float4 background = RGB{ 30, 30, 30 };
    Float4x4 custom_data;

    String skybox_texture_name = "/";
    String target_texture_name = "/";
    String shader_name = "/";

    Texture screen_texture;
    Texture color_texture;
    Texture depth_texture;
    Texture depth_staging;
    Texture index_texture;
    Texture index_staging;

    Camera();

    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;

    void update_aspect_ratio( Int2 size );
    void set_forward( Float3 const& dir );
    void set_up( Float3 const& dir );

    Float3 forward() const;
    Float3 right() const;
    Float3 up() const;

    void move_forward( float delta_time );
    void move_back( float delta_time );
    void move_right( float delta_time );
    void move_left( float delta_time );
    void move_up( float delta_time );
    void move_down( float delta_time );

    void rotate( Float2 mouse_pos, Float2 frame_center, float vertical_angle_limit = 85.0f );

    Float4x4 view_matrix() const;
    Float4x4 projection_matrix() const;
    Float4x4 camera_matrix() const;

    bool can_see( Float3 const& point ) const;

    void resize( Int2 new_size );
    void resize_staging( Int2 new_size );
    Int2 resolution() const;
    void clear_targets();

private:
    Float3 m_forward = { 0.0f, 0.0f, 1.0f };
    Float3 m_up = { 0.0f, 1.0f, 0.0f };
};
}
