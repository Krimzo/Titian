#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

namespace titian
{
struct GUISectionMaterialEditor : GUISection
{
    Camera camera;
    Texture render_texture;
    Texture depth_texture;

    bool was_focused = true;
    float2 initial_camera_info = {40, 30};
    float2 camera_info = initial_camera_info;

    String selected_material = {};

    GUISectionMaterialEditor();

    void render_gui() override;

  private:
    float4 m_new_mat_color = kl::colors::WHITE;

    void display_materials(Scene& scene);
    void update_material_camera(Scene& scene);
    void render_selected_material(Material& material, int2 viewport_size);
    void show_material_properties(Material* material);
};
} // namespace titian
