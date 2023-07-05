#pragma once

#include "state/logger_state.h"
#include "state/render_state.h"
#include "state/gui_state.h"


inline const std::string builtin_path = "./builtin/";

struct editor_state
{
    // Render features
    bool render_wireframe = false;
    bool v_sync = true;

    // Utility
    logger_state logger = {};
    kl::timer timer = {};

    // Window
    kl::object<kl::window> window = new kl::window({ 1600, 900 }, "Titian");
    kl::object<kl::gpu> gpu = new kl::gpu((HWND) *window,
#ifdef NDEBUG
        false
#else
        true
#endif
    );

    // Graphics states
    struct raster_states_t
    {
        kl::dx::raster_state wireframe = nullptr;
        kl::dx::raster_state solid_cull = nullptr;
        kl::dx::raster_state solid = nullptr;
        kl::dx::raster_state shadow = nullptr;
    } raster_states = {};

    struct depth_states_t
    {
        kl::dx::depth_state disabled = nullptr;
        kl::dx::depth_state enabled = nullptr;
    } depth_states = {};

    struct sampler_states_t
    {
        kl::dx::sampler_state linear = nullptr;
        kl::dx::sampler_state shadow = nullptr;
    } sampler_states = {};

    // Shaders
    struct render_shaders_t
    {
        kl::render_shaders shadow = {};
        kl::render_shaders skybox = {};
        kl::render_shaders entity = {};
        kl::render_shaders collider = {};
        kl::render_shaders postprocess = {};
    } render_shaders;

    // Default Components
    std::unordered_map<std::string, kl::object<kl::mesh>> default_meshes = {};
    std::unordered_map<std::string, kl::object<kl::material>> default_materials = {};

    // Render
    kl::object<render_state> render_state = new ::render_state(gpu, window->size());
    kl::object<kl::scene> scene = nullptr;

    // GUI
    gui_state gui_state = { gpu };

    editor_state();
    ~editor_state();

    editor_state(editor_state&) = delete;
    editor_state(editor_state&&) = delete;

    void operator=(editor_state&) = delete;
    void operator=(editor_state&&) = delete;

    void change_scene(kl::object<kl::scene> scene);
};
