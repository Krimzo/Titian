#include "main.h"


titian::ShaderStates::ShaderStates(kl::GPU* gpu)
{
    auto load_shader = [&](kl::RenderShaders& shader, const char* filename)
	{
        const std::string source = kl::read_file_string(kl::format("builtin/shaders/", filename));
        shader = gpu->create_render_shaders(source);
        kl::assert(shader, "Failed to init [", filename, "] shaders");
	};

    WorkQueue queue;
    queue.add_task([&] { load_shader(solid_pass, "solid_pass.hlsl"); });
    queue.add_task([&] { load_shader(solid_lit_pass, "solid_lit_pass.hlsl"); });
    queue.add_task([&] { load_shader(shadow_pass, "shadow_pass.hlsl"); });
    queue.add_task([&] { load_shader(skybox_pass, "skybox_pass.hlsl"); });
    queue.add_task([&] { load_shader(scene_pass, "scene_pass.hlsl"); });
	queue.add_task([&] { load_shader(post_pass, "post_pass.hlsl"); });
    queue.add_task([&] { load_shader(outline_pass, "outline_pass.hlsl"); });
    queue.add_task([&] { load_shader(display_pass, "display_pass.hlsl"); });
    queue.finalize();
}
