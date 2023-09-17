export module load_meshes;

export import sandbox;

kl::Object<kl::Mesh> load_mesh(EditorState* state, const std::string& path);

export void load_meshes(EditorState* state)
{
    // None yet
}

kl::Object<kl::Mesh> load_mesh(EditorState* state, const std::string& path)
{
    const kl::MeshData mesh_data = kl::parse_obj_file(path, true);
    return new kl::Mesh(&state->gpu, &state->scene, mesh_data);
}
