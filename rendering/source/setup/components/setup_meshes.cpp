#include "setup/setup_state.h"


kl::ref<kl::mesh> parse_mesh(state_machine* state, const std::string& name);

void setup_meshes(state_machine* state)
{
    state->meshes["cube"] = parse_mesh(state, "cube.obj");
    state->meshes["sphere"] = parse_mesh(state, "sphere.obj");
    state->meshes["monke"] = parse_mesh(state, "monke.obj");

    /* 50_bmg */
    state->meshes["bmg_bullet"] = parse_mesh(state, "50_bmg/bullet.obj");
    state->meshes["bmg_casing"] = parse_mesh(state, "50_bmg/casing.obj");
    state->meshes["bmg_primer"] = parse_mesh(state, "50_bmg/primer.obj");
    state->meshes["bmg_primer_fired"] = parse_mesh(state, "50_bmg/primer_fired.obj");
    /* ------ */
}

kl::ref<kl::mesh> parse_mesh(state_machine* state, const std::string& name)
{
    const kl::mesh_data vertices = kl::files::parse_mesh(resouce_folder + "meshes/" + name, true);
    return kl::ref<kl::mesh>(new kl::mesh(*state->gpu, *state->scene, vertices));
}
