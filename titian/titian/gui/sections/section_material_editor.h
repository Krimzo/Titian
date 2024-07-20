#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionMaterialEditor : public GUISection
    {
    public:
        Ref<Camera> camera = nullptr;
        Ref<Texture> render_texture = nullptr;
        Ref<Texture> depth_texture = nullptr;

        bool was_focused = true;
        int last_scroll = 0;
        kl::Float2 initial_camera_info = { 40, 30 };
        kl::Float2 camera_info = initial_camera_info;

        String selected_material = {};

        GUISectionMaterialEditor(const LayerPackage& package);

        void render_gui() override;

    private:
        void display_materials(kl::GPU* gpu, Scene* scene);
        void render_selected_material(Scene* scene, kl::GPU* gpu, Material* material, kl::Int2 viewport_size);
        void show_material_properties(Scene* scene, Material* material);
        void update_material_camera();
    };
}
