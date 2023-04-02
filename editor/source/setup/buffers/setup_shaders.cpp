#include "setup/setup_state.h"


std::string parse_shader(const std::string& name);

void setup_shaders(editor_state* state)
{
    auto shadow_shaders = parse_shader("shadow_render.hlsl");
    state->render_shaders["shadow"] = state->gpu->create_render_shaders(shadow_shaders);

    auto skybox_shaders = parse_shader("skybox_render.hlsl");
    state->render_shaders["skybox"] = state->gpu->create_render_shaders(skybox_shaders);

    auto render_shaders = parse_shader("entity_render.hlsl");
    state->render_shaders["entity"] = state->gpu->create_render_shaders(render_shaders);

    auto collider_shaders = parse_shader("collider_render.hlsl");
    state->render_shaders["collider"] = state->gpu->create_render_shaders(collider_shaders);

    auto postprocess_shaders = parse_shader("postprocess_render.hlsl");
    state->render_shaders["postprocess"] = state->gpu->create_render_shaders(postprocess_shaders);
}

std::string parse_shader(const std::string& name)
{
    return kl::files::read_string(builtin_path + "/shaders/" + name);
}
