#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionMeshEditor : public GUISection
    {
    public:
        Ref<Camera> camera = nullptr;
        Ref<Texture> render_texture = nullptr;
        Ref<Texture> depth_texture = nullptr;

        Float4 line_color = kl::colors::WHITE;
        Float3 sun_direction = { 0.0f, -1.0f, 0.0f };

        bool was_focused = true;
        int last_scroll = 0;
        Float2 initial_camera_info = { 200.0f, 10.0f };
        Float2 camera_info = initial_camera_info;

        String selected_mesh = "/";

        GUISectionMeshEditor(const LayerPackage& package);

        void render_gui() override;

    private:
        int m_selected_vertex_index = -1;
        int m_starting_vertex_index = 0;
        int m_vertex_display_count = 6;
        int m_last_scroll = 0;
        int m_new_vertex_index = 0;

        void display_meshes(kl::GPU* gpu, Scene* scene);
        void update_mesh_camera();
        void render_selected_mesh(kl::GPU* gpu, const Mesh* mesh, Int2 viewport_size);
        void render_gizmos(Mesh* mesh);
        void show_mesh_properties(Mesh* mesh);
    };
}
