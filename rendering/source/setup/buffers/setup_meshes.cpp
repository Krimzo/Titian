#include "setup/setup_state.h"


std::vector<kl::vertex> parse_mesh(const std::string& name);

void setup_meshes(state_machine* state)
{
	auto cube_mesh = parse_mesh("cube.obj");
	state->meshes["cube"] = state->gpu->new_vertex_buffer(cube_mesh);

	auto sphere_mesh = parse_mesh("sphere.obj");
	state->meshes["sphere"] = state->gpu->new_vertex_buffer(sphere_mesh);

	auto monke_mesh = parse_mesh("monke.obj");
	state->meshes["monke"] = state->gpu->new_vertex_buffer(monke_mesh);
}

std::vector<kl::vertex> parse_mesh(const std::string& name)
{
	return kl::files::parse_mesh(resouce_folder + "meshes/" + name, true);
}
