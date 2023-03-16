#include "sandbox.h"


kl::dx::shader_view parse_texture(editor_state* state, const std::string& path);

void load_textures(editor_state* state)
{
    state->scene->textures["checkers"] = parse_texture(state, "../preview/textures/checkers.png");
    state->scene->textures["dogo"] = parse_texture(state, "../preview/textures/dogo.png");
    state->scene->textures["concrete_normal"] = parse_texture(state, "../preview/textures/concrete_normal.png");
    state->scene->textures["concrete_roughness"] = parse_texture(state, "../preview/textures/concrete_roughness.png");
}

kl::dx::shader_view parse_texture(editor_state* state, const std::string& path)
{
    auto texture = state->gpu->create_texture(kl::image(path));
    return state->gpu->create_shader_view(texture, nullptr);
}
