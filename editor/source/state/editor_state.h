#pragma once

#include "state/logger_state.h"
#include "state/render_state.h"
#include "state/gui_state.h"
#include "state/script_state.h"


inline const std::string builtin_path = "./builtin/";

struct editor_state
{
    // System
    kl::timer timer = {};
    kl::object<kl::window> window = new kl::window({ 1600, 900 }, "Titian");
    kl::object<kl::gpu> gpu = new kl::gpu((HWND) *window,
#ifdef NDEBUG
        false
#else
        true
#endif
    );

    // Render features
    bool game_running = false;
    bool render_wireframe = false;
    bool v_sync = true;

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
        kl::render_shaders shadow_pass = {};
        kl::render_shaders skybox_sample = {};
        kl::render_shaders object_single = {};
        kl::render_shaders object_material = {};
        kl::render_shaders object_full = {};
        kl::render_shaders postprocess_pass = {};
    } render_shaders;

    // Scene components
    struct default_mesh_t
    {
        kl::object<kl::mesh> cube = {};
        kl::object<kl::mesh> sphere = {};
        kl::object<kl::mesh> capsule = {};
        kl::object<kl::mesh> monke = {};
    } default_mesh = {};
    struct default_material_t
    {
        kl::object<kl::material> white = {};
    } default_material = {};
    kl::object<kl::scene> scene = new kl::scene();

    // Engine states
    kl::object<logger_state> logger_state = new ::logger_state();
    kl::object<render_state> render_state = new ::render_state(gpu, window->size());
    kl::object<gui_state> gui_state = new ::gui_state(gpu);
    kl::object<script_state> script_state = new ::script_state();

    editor_state();
    ~editor_state();

    editor_state(editor_state&) = delete;
    editor_state(editor_state&&) = delete;

    void operator=(editor_state&) = delete;
    void operator=(editor_state&&) = delete;

    void change_scene(kl::object<kl::scene> scene);
};
