export module load_textures;

export import sandbox;

kl::Object<kl::Texture> parse_texture(EditorState* state, const std::string& path);

export void load_textures(EditorState* state)
{
    state->scene->textures["checkers"] = parse_texture(state, RESOURCE_PATH + "textures/checkers.png");
    state->scene->textures["dogo"] = parse_texture(state, RESOURCE_PATH + "textures/dogo.png");
    state->scene->textures["concrete_normal"] = parse_texture(state, RESOURCE_PATH + "textures/concrete_normal.png");
    state->scene->textures["concrete_roughness"] = parse_texture(state, RESOURCE_PATH + "textures/concrete_roughness.png");
}

kl::Object<kl::Texture> parse_texture(EditorState* state, const std::string& path)
{
    kl::Object texture = new kl::Texture(&state->gpu, kl::Image(path));
    texture->create_shader_view();
    return texture;
}
