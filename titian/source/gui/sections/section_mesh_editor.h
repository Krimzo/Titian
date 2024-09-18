#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    struct GUISectionMeshEditor : GUISection
    {
        Ref<Camera> camera;
        Ref<Texture> render_texture;
        Ref<Texture> depth_texture;

        Float4 line_color = kl::colors::WHITE;
        Float3 sun_direction = { 0.0f, -1.0f, 0.0f };

        bool was_focused = true;
        Float2 initial_camera_info = { 200.0f, 10.0f };
        Float2 camera_info = initial_camera_info;

        String selected_mesh = "/";

        GUISectionMeshEditor();

        void render_gui() override;

    private:
        int m_selected_vertex_index = -1;
        int m_starting_vertex_index = 0;
        int m_vertex_display_count = 6;
        int m_new_vertex_index = 0;

        float m_plane_size = 10.0f;
        int m_plane_complexity = 10;
        float m_cube_size = 1.0f;
        float m_sphere_radius = 1.0f;
        int m_sphere_complexity = 3;
        bool m_sphere_smooth = true;
        float m_capsule_radius = 1.0f;
        float m_capsule_height = 2.0f;
        int m_capsule_sectors = 16;
        int m_capsule_rings = 16;

        void display_meshes(Scene* scene);
        void update_mesh_camera(Scene* scene);
        void render_selected_mesh(Mesh* mesh, Int2 viewport_size);
        void show_mesh_properties(Mesh* mesh);
        void render_gizmos(Mesh* mesh);
    };
}
