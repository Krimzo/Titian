#include "setup/setup_state.h"


std::vector<kl::vertex> parse_mesh(const std::string& name);

void setup_meshes(state_machine* state)
{
    state->meshes["screen"] = state->gpu->generate_screen_mesh();

    auto cube_mesh = parse_mesh("cube.obj");
    state->meshes["cube"] = state->gpu->new_vertex_buffer(cube_mesh);

    auto sphere_mesh = parse_mesh("sphere.obj");
    state->meshes["sphere"] = state->gpu->new_vertex_buffer(sphere_mesh);

    auto monke_mesh = parse_mesh("monke.obj");
    state->meshes["monke"] = state->gpu->new_vertex_buffer(monke_mesh);

    /* 50_bmg */
    auto bmg_bullet = parse_mesh("50_bmg/bullet.obj");
    state->meshes["bmg_bullet"] = state->gpu->new_vertex_buffer(bmg_bullet);

    auto bmg_casing = parse_mesh("50_bmg/casing.obj");
    state->meshes["bmg_casing"] = state->gpu->new_vertex_buffer(bmg_casing);

    auto bmg_primer = parse_mesh("50_bmg/primer.obj");
    state->meshes["bmg_primer"] = state->gpu->new_vertex_buffer(bmg_primer);

    auto bmg_primer_fired = parse_mesh("50_bmg/primer_fired.obj");
    state->meshes["bmg_primer_fired"] = state->gpu->new_vertex_buffer(bmg_primer_fired);
    /* ------ */
}

std::vector<kl::vertex> parse_mesh(const std::string& name)
{
    return kl::files::parse_mesh(resouce_folder + "meshes/" + name, true);
}
