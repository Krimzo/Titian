#include "setup/setup_state.h"


std::string parse_shader(const std::string& name);

void setup_shaders(state_machine* state)
{
    auto shadow_shaders = parse_shader("shadow_render.hlsl");
    state->shaders["shadow"] = state->gpu->create_render_shaders(shadow_shaders);

    auto skybox_shaders = parse_shader("skybox_render.hlsl");
    state->shaders["skybox"] = state->gpu->create_render_shaders(skybox_shaders);

    auto render_shaders = parse_shader("entity_render.hlsl");
    state->shaders["entity"] = state->gpu->create_render_shaders(render_shaders);

    auto postprocess_shaders = parse_shader("postprocess_render.hlsl");
    state->shaders["postprocess"] = state->gpu->create_render_shaders(postprocess_shaders);
}

std::string parse_shader(const std::string& name)
{
    return kl::files::read_string(resouce_folder + "shaders/" + name);
}
