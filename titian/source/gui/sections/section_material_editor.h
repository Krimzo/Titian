#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    struct GUISectionMaterialEditor : GUISection
    {
        Ref<Camera> camera;
        Ref<Texture> render_texture;
        Ref<Texture> depth_texture;

        bool was_focused = true;
        Float2 initial_camera_info = { 40, 30 };
        Float2 camera_info = initial_camera_info;

        String selected_material = {};

        GUISectionMaterialEditor();

        void render_gui() override;

    private:
        Float4 m_new_mat_color = kl::colors::WHITE;

        void display_materials(Scene* scene);
        void update_material_camera(Scene* scene);
        void render_selected_material(Material* material, Int2 viewport_size);
        void show_material_properties(Material* material);
    };
}
