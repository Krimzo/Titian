#include "main.h"


titian::ShaderStates::ShaderStates(kl::GPU* gpu)
{
    const std::initializer_list<std::pair<kl::RenderShaders&, const char*>> shaders = {
        { solid_pass, "solid_pass.hlsl" },
        { solid_lit_pass, "solid_lit_pass.hlsl" },
        { shadow_pass, "shadow_pass.hlsl" },
        { skybox_pass, "skybox_pass.hlsl" },
        { scene_pass, "scene_pass.hlsl" },
        { outline_pass, "outline_pass.hlsl" },
        { display_pass, "display_pass.hlsl" },
    };
    std::for_each(std::execution::par, shaders.begin(), shaders.end(), [&](auto entry)
    {
        const std::string source = kl::read_file_string(kl::format("builtin/shaders/", entry.second));
        entry.first = gpu->create_render_shaders(source);
        kl::assert(entry.first, kl::format("Failed to init [", entry.second, "] shaders."));
    });
}
