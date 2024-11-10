#pragma once

#include "tracing.h"


namespace titian
{
struct GPUTracer
{
    kl::Window& window;
    TracingScene const& scene;

    GPUTracer( kl::Window& window, TracingScene const& scene, Int2 resolution );

    void render();

private:
    kl::GPU m_gpu;
    kl::Timer m_timer;

    dx::Texture m_target;
    dx::AccessView m_target_access_view;

    dx::Texture m_target_copy;
    dx::AccessView m_target_copy_access_view;
    dx::ShaderView m_target_copy_shader_view;

    kl::ComputeShader m_compute_shader;
    kl::ComputeShader m_copy_shader;
    kl::Shaders m_display_shaders;

    dx::ShaderView m_entities_shader_view;
    dx::ShaderView m_triangles_shader_view;
    dx::ShaderView m_skybox_shader_view;

    dx::Buffer m_screen_mesh;
    dx::SamplerState m_linear_sampler;

    int m_iterations = 0;

    void create_scene_buffers();
    void handle_input();
    void save_screenshot( StringRef const& path );

    void compute_pass();
    void copy_pass();
    void display_pass();
};
}
