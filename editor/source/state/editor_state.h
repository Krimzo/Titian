#pragma once

#include "state/logger_state.h"
#include "state/render_state.h"
#include "state/gui_state.h"


inline const std::string builtin_path = "./builtin/";

struct editor_state
{
    // Utility
    logger_state logger = {};
    kl::timer timer = {};

    // Window
    kl::ref<kl::window> window = kl::make<kl::window>(kl::int2(1600, 900), "Titian");
    kl::ref<kl::gpu> gpu = kl::make<kl::gpu>((HWND) *window);

    // Graphics states
    std::unordered_map<std::string, kl::dx::depth_state> depth_states = {};
    std::unordered_map<std::string, kl::dx::sampler_state> sampler_states = {};
    std::unordered_map<std::string, kl::dx::raster_state> raster_states = {};

    // Graphics buffers
    std::unordered_map<std::string, kl::render_shaders> shaders = {};

    // Default Components
    std::unordered_map<std::string, kl::ref<kl::mesh>> default_meshes = {};
    std::unordered_map<std::string, kl::ref<kl::material>> default_materials = {};

    // Render
    kl::ref<render_state> render_state = kl::make<::render_state>(gpu, window->size());
    kl::ref<kl::scene> scene = kl::make<kl::scene>();

    // GUI
    gui_state gui_state = { gpu };

    editor_state();
    virtual ~editor_state();

    editor_state(editor_state&) = delete;
    editor_state(editor_state&&) = delete;

    void operator=(editor_state&) = delete;
    void operator=(editor_state&&) = delete;
};
