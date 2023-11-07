#include "main.h"


titian::ShaderStates::ShaderStates(kl::GPU* gpu)
{
    const auto parse_render_shaders = [&](const std::string& name)
    {
        const std::string source = kl::read_file_string("builtin/shaders/" + name);
        return gpu->create_render_shaders(source);
    };

    shadow_pass = parse_render_shaders("shadow_pass.hlsl");
    skybox_pass = parse_render_shaders("skybox_pass.hlsl");
    material_pass = parse_render_shaders("material_pass.hlsl");
    unlit_pass = parse_render_shaders("unlit_pass.hlsl");
    lit_pass = parse_render_shaders("lit_pass.hlsl");
    pp_pass = parse_render_shaders("pp_pass.hlsl");
    display_pass = parse_render_shaders("display_pass.hlsl");

    kl::assert(shadow_pass, "Failed to init SHADOW_PASS render shaders.");
    kl::assert(skybox_pass, "Failed to init SKYBOX_PASS render shaders.");
    kl::assert(material_pass, "Failed to init MATERIAL_PASS render shaders.");
    kl::assert(unlit_pass, "Failed to init UNLIT_PASS render shaders.");
    kl::assert(lit_pass, "Failed to init LIT_PASS render shaders.");
    kl::assert(pp_pass, "Failed to init PP_PASS render shaders.");
    kl::assert(display_pass, "Failed to init DISPLAY_PASS render shaders.");
}
