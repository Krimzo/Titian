export module load_skyboxes;

export import sandbox;

kl::Object<kl::Texture> parse_skybox(EditorState* state, const std::string& path);

export void load_skyboxes(EditorState* state)
{
    state->scene->textures["clouds"] = parse_skybox(state, RESOURCE_PATH + "skyboxes/clouds.png");
    state->scene->textures["night"] = parse_skybox(state, RESOURCE_PATH + "skyboxes/night.png");
}

kl::Object<kl::Texture> parse_skybox(EditorState* state, const std::string& path)
{
    kl::Object texture = new kl::Texture(&state->gpu, kl::Image(path), true);
    texture->create_shader_view();
    return texture;
}
