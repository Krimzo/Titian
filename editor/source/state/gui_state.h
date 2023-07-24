#pragma once

#include "klib.h"


struct gui_state
{
    bool is_viewport_focused = false;
    bool render_collider = true;

    int gizmo_operation = 0;
    int gizmo_mode = 1;
    int explorer_icon_size = 65;

    kl::int2 viewport_size = {};

    kl::float4 selection_color = kl::colors::orange;
    kl::float4 collider_color = kl::color(90, 225, 100);

    std::string builtin_path = "./builtin/";
    std::string explorer_path = std::filesystem::absolute(".").string();

    struct textures_t
    {
        kl::object<kl::texture> folder = {};
        kl::object<kl::texture> folder_empty = {};
        kl::object<kl::texture> folder_parent = {};
        kl::object<kl::texture> file = {};
        kl::object<kl::texture> code = {};
        kl::object<kl::texture> script = {};
        kl::object<kl::texture> mesh = {};
        kl::object<kl::texture> texture = {};
        kl::object<kl::texture> scene = {};
    } textures;

    // Mesh editor
    struct mesh_editor_t
    {
        kl::camera camera = {};
        kl::object<kl::mesh> selected_mesh = {};
        kl::object<kl::texture> render_texture = {};
        kl::render_shaders shaders = {};
    } mesh_editor;

    // Material editor
    struct material_editor_t
    {
        kl::camera camera = {};
        kl::object<kl::material> selected_material = {};
        kl::object<kl::texture> selected_texture = {};
        kl::object<kl::texture> render_texture = {};
        kl::render_shaders shaders = {};
    } material_editor;

    gui_state(kl::object<kl::gpu> gpu);

    gui_state(gui_state&) = delete;
    gui_state(gui_state&&) = delete;

    void operator=(gui_state&) = delete;
    void operator=(gui_state&&) = delete;
};
