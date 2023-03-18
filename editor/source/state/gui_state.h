#pragma once

#include "klib.h"


class gui_state
{
    kl::ref<kl::gpu> gpu_ = nullptr;

public:
    bool is_viewport_focused = false;
    bool render_collider = true;

    int gizmo_operation = 0;
    int gizmo_mode = 1;

    kl::int2 viewport_size = {};

    kl::float4 selection_color = kl::colors::orange;
    kl::float4 collider_color = kl::color(90, 225, 100);

    std::string builtin_path = "./builtin/";
    std::string explorer_path = ".";

    std::unordered_map<std::string, kl::ref<kl::texture>> textures = {};

    gui_state(kl::ref<kl::gpu> gpu);
    virtual ~gui_state();

    gui_state(gui_state&) = delete;
    gui_state(gui_state&&) = delete;

    void operator=(gui_state&) = delete;
    void operator=(gui_state&&) = delete;
};
