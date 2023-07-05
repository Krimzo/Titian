#include "sandbox.h"


kl::object<kl::texture> parse_skybox(editor_state* state, const std::string& path);

void load_skyboxes(editor_state* state)
{
    state->scene->textures["clouds"] = parse_skybox(state, resource_path + "skyboxes/clouds.png");
    state->scene->textures["night"] = parse_skybox(state, resource_path + "skyboxes/night.png");
}

kl::object<kl::texture> parse_skybox(editor_state* state, const std::string& path)
{
    kl::object<kl::texture> texture = new kl::texture(&state->gpu, kl::image(path), true);
    texture->create_shader_view();
    return texture;
}
