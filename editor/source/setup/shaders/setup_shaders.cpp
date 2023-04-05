#include "setup/setup_state.h"


kl::render_shaders parse_render_shaders(editor_state* state, const std::string& name);

void setup_shaders(editor_state* state)
{
    state->render_shaders.shadow = parse_render_shaders(state, "shadow_render.hlsl");
    state->render_shaders.skybox = parse_render_shaders(state, "skybox_render.hlsl");
    state->render_shaders.entity = parse_render_shaders(state, "entity_render.hlsl");
    state->render_shaders.collider = parse_render_shaders(state, "collider_render.hlsl");
    state->render_shaders.postprocess = parse_render_shaders(state, "postprocess_render.hlsl");
}

kl::render_shaders parse_render_shaders(editor_state* state, const std::string& name)
{
    const auto source = kl::files::read_string(builtin_path + "/shaders/" + name);
    return state->gpu->create_render_shaders(source);
}
