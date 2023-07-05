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
    std::string explorer_path = ".";

    std::unordered_map<std::string, kl::object<kl::texture>> textures = {};

    gui_state(kl::object<kl::gpu> gpu);

    gui_state(gui_state&) = delete;
    gui_state(gui_state&&) = delete;

    void operator=(gui_state&) = delete;
    void operator=(gui_state&&) = delete;
};
