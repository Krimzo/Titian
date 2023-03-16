#pragma once

#include "state/logger_state.h"
#include "state/render_state.h"
#include "state/gui_state.h"


inline const std::string builtin_path = "../builtin/";

struct editor_state
{
    // Utility
    logger_state logger = {};
    kl::timer timer = {};

    // Window
    kl::ref<kl::window> window = {};
    kl::ref<kl::gpu> gpu = {};

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
    kl::ref<render_state> render_state = {};
    kl::ref<kl::scene> scene = {};

    // GUI
    gui_state gui_state = {};

    editor_state();
    virtual ~editor_state();

    editor_state(editor_state&) = delete;
    editor_state(editor_state&&) = delete;

    void operator=(editor_state&) = delete;
    void operator=(editor_state&&) = delete;
};
