#include "sandbox.h"


kl::ref<kl::texture> parse_texture(editor_state* state, const std::string& path);

void load_textures(editor_state* state)
{
    state->scene->textures["checkers"] = parse_texture(state, resource_path + "textures/checkers.png");
    state->scene->textures["dogo"] = parse_texture(state, resource_path + "textures/dogo.png");
    state->scene->textures["concrete_normal"] = parse_texture(state, resource_path + "textures/concrete_normal.png");
    state->scene->textures["concrete_roughness"] = parse_texture(state, resource_path + "textures/concrete_roughness.png");
}

kl::ref<kl::texture> parse_texture(editor_state* state, const std::string& path)
{
    auto texture = kl::make<kl::texture>(state->gpu.get(), kl::image(path));
    texture->create_shader_view();
    return texture;
}
