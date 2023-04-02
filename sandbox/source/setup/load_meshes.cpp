#include "sandbox.h"


kl::ref<kl::mesh> load_mesh(editor_state* state, const std::string& path);

void load_meshes(editor_state* state)
{
    state->scene->meshes["bmg_bullet"] = load_mesh(state, resource_path + "meshes/bullet.obj");
    state->scene->meshes["bmg_casing"] = load_mesh(state, resource_path + "meshes/casing.obj");
    state->scene->meshes["bmg_primer"] = load_mesh(state, resource_path + "meshes/primer.obj");
    state->scene->meshes["bmg_primer_fired"] = load_mesh(state, resource_path + "meshes/primer_fired.obj");
}

kl::ref<kl::mesh> load_mesh(editor_state* state, const std::string& path)
{
    const auto mesh_data = kl::files::parse_mesh(path, true);
    return kl::make<kl::mesh>(state->scene.get(), state->gpu.get(), mesh_data);
}
