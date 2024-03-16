#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionMaterialEditor : public GUISection
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        kl::Object<Camera> camera = nullptr;
        kl::Object<Texture> render_texture = nullptr;
        kl::Object<Texture> depth_texture = nullptr;

        bool was_focused = true;
        int last_scroll = 0;
        kl::Float2 initial_camera_info = { 40, 30 };
        kl::Float2 camera_info = initial_camera_info;

        std::string selected_material = {};

        GUISectionMaterialEditor(EditorLayer* editor_layer, GUILayer* gui_layer);

        void render_gui() override;

    private:
        void display_materials(kl::GPU* gpu, Scene* scene);
        void render_selected_material(Scene* scene, kl::GPU* gpu, Material* material, kl::Int2 viewport_size);
        void show_material_properties(Scene* scene, Material* material);
        void update_material_camera();
    };
}
