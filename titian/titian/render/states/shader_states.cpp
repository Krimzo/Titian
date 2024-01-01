#include "main.h"


titian::ShaderStates::ShaderStates(kl::GPU* gpu)
{
    const auto parse_render_shaders = [&](const std::string& name)
    {
        const std::string source = kl::read_file_string("builtin/shaders/" + name);
        return gpu->create_render_shaders(source);
    };

    solid_pass = parse_render_shaders("solid_pass.hlsl");
    shadow_pass = parse_render_shaders("shadow_pass.hlsl");
    skybox_pass = parse_render_shaders("skybox_pass.hlsl");
    scene_pass = parse_render_shaders("scene_pass.hlsl");
    outline_pass = parse_render_shaders("outline_pass.hlsl");
    display_pass = parse_render_shaders("display_pass.hlsl");
    material_editor_pass = parse_render_shaders("material_editor_pass.hlsl");
    mesh_editor_pass = parse_render_shaders("mesh_editor_pass.hlsl");

    kl::assert(solid_pass, "Failed to init SOLID_PASS shaders.");
    kl::assert(shadow_pass, "Failed to init SHADOW_PASS shaders.");
    kl::assert(skybox_pass, "Failed to init SKYBOX_PASS shaders.");
    kl::assert(scene_pass, "Failed to init SCENE_PASS shaders.");
    kl::assert(outline_pass, "Failed to init OUTLINE_PASS shaders.");
    kl::assert(display_pass, "Failed to init DISPLAY_PASS shaders.");
    kl::assert(material_editor_pass, "Failed to init MATERIAL_EDITOR_PASS shaders.");
    kl::assert(mesh_editor_pass, "Failed to init MESH_EDITOR_PASS shaders.");
}
