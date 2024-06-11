#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionMeshEditor : public GUISection
    {
    public:
        kl::Object<Camera> camera = nullptr;
        kl::Object<Texture> render_texture = nullptr;
        kl::Object<Texture> depth_texture = nullptr;

        kl::Float4 line_color = kl::colors::WHITE;
        kl::Float3 sun_direction = { 0.0f, -1.0f, 0.0f };

        bool was_focused = true;
        int last_scroll = 0;
        kl::Float2 initial_camera_info = { 200.0f, 10.0f };
        kl::Float2 camera_info = initial_camera_info;

        std::string selected_mesh = "/";

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
        void render_selected_mesh(kl::GPU* gpu, const Mesh* mesh, kl::Int2 viewport_size);
        void render_gizmos(Mesh* mesh);
        void show_mesh_properties(Mesh* mesh);
    };
}
