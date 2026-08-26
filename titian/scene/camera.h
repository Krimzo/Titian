#pragma once

#include "scene/entity.h"

namespace titian
{
enum CameraType : int32_t
{
    PERSPECTIVE = 0,
    ORTHOGRAPHIC = 1,
};
} // namespace titian

namespace titian
{
struct Camera : Entity
{
    CameraType camera_type = CameraType::PERSPECTIVE;

    bool enabled = true;
    bool v_sync = false;
    bool render_wireframe = false;

    float field_of_view = 110.0f;
    float aspect_ratio = 1.7778f;
    float width = 16.0f;
    float height = 9.0f;
    float near_plane = 0.01f;
    float far_plane = 100.0f;
    float sensitivity = 0.1f;
    float speed = 2.0f;

    float4 background = RGB{30, 30, 30};
    float4x4 custom_data;

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

    void serialize(Serializer& serializer) const override;
    void deserialize(Serializer const& serializer) override;

    void update_aspect_ratio(int2 size);
    void set_forward(float3 const& dir);
    void set_up(float3 const& dir);

    float3 forward() const;
    float3 right() const;
    float3 up() const;

    void move_forward(float delta_time);
    void move_back(float delta_time);
    void move_right(float delta_time);
    void move_left(float delta_time);
    void move_up(float delta_time);
    void move_down(float delta_time);

    void rotate(float2 mouse_pos, float2 frame_center, float vertical_angle_limit = 85.0f);

    float4x4 view_matrix() const;
    float4x4 projection_matrix() const;
    float4x4 camera_matrix() const;

    bool can_see(float3 const& point) const;

    void resize(int2 new_size);
    void resize_staging(int2 new_size);
    int2 resolution() const;
    void clear_targets();

  private:
    float3 m_forward = {0.0f, 0.0f, 1.0f};
    float3 m_up = {0.0f, 1.0f, 0.0f};
};
} // namespace titian
