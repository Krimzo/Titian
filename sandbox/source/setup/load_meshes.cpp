#include "sandbox.h"


kl::object<kl::mesh> load_mesh(editor_state* state, const std::string& path);

void load_meshes(editor_state* state)
{
    // None yet
}

kl::object<kl::mesh> load_mesh(editor_state* state, const std::string& path)
{
    const auto mesh_data = kl::parse_obj_file(path, true);
    return new kl::mesh(&state->gpu, &state->scene, mesh_data);
}
