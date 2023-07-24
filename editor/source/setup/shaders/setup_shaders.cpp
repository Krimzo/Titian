#include "setup/setup_state.h"


kl::render_shaders parse_render_shaders(editor_state* state, const std::string& name);

void setup_shaders(editor_state* state)
{
    state->render_shaders.shadow_pass = parse_render_shaders(state, "shadow_pass.hlsl");
    state->render_shaders.skybox_sample = parse_render_shaders(state, "skybox_sample.hlsl");
    state->render_shaders.object_single = parse_render_shaders(state, "object_single.hlsl");
    state->render_shaders.object_material = parse_render_shaders(state, "object_material.hlsl");
    state->render_shaders.object_full = parse_render_shaders(state, "object_full.hlsl");
    state->render_shaders.postprocess_pass = parse_render_shaders(state, "postprocess_pass.hlsl");
}

kl::render_shaders parse_render_shaders(editor_state* state, const std::string& name)
{
    const auto source = kl::read_file_string(builtin_path + "/shaders/" + name);
    return state->gpu->create_render_shaders(source);
}
