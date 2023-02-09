#pragma once

#include "state/render_state.h"


inline const std::string resouce_folder = "../resource/";

struct state_machine
{
    // System
    kl::ref<kl::window> window = {};
    kl::ref<kl::gpu>       gpu = {};
    kl::timer            timer = {};

    // Graphics states
    std::unordered_map<std::string, kl::dx::depth_state>     depth_states = {};
    std::unordered_map<std::string, kl::dx::sampler_state> sampler_states = {};
    std::unordered_map<std::string, kl::dx::raster_state>   raster_states = {};

    // Graphics buffers
    std::unordered_map<std::string, kl::shaders>          shaders = {};
    std::unordered_map<std::string, kl::dx::shader_view> textures = {};
    std::unordered_map<std::string, kl::dx::shader_view> skyboxes = {};

    // Components
    std::unordered_map<std::string, kl::ref<kl::mesh>>        meshes = {};
    std::unordered_map<std::string, kl::ref<kl::material>> materials = {};

    // Render
    kl::ref<render_state> render_state = {};
    kl::ref<kl::scene>           scene = {};

    state_machine();
    ~state_machine();

    state_machine(state_machine&) = delete;
    state_machine(state_machine&&) = delete;

    void operator=(state_machine&) = delete;
    void operator=(state_machine&&) = delete;
};
