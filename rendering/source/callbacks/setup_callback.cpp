#include "callbacks/callbacks.h"


void setup_test_scene(state_machine* state);

void setup_callback(state_machine* state)
{
	const std::string resouce_folder = "../resource/";

	// Shaders
	state->shaders["render"] = state->gpu->new_shaders(kl::files::read_string(resouce_folder + "shaders/render.hlsl"));

	// Meshes
	state->meshes["cube"] = state->gpu->new_vertex_buffer(kl::files::parse_mesh(resouce_folder + "meshes/cube.obj", true));

	// Textures
	auto temp_texture = state->gpu->new_texture(kl::image(resouce_folder + "textures/checkers.png"));
	state->textures["checkers"] = state->gpu->new_shader_view(temp_texture);

	// Scene
	setup_test_scene(state);
}

void setup_test_scene(state_machine* state)
{
	auto scene = kl::make<::scene>();

	scene->sun_direction = { -1, -1, 1 };

	auto test_entity = kl::make<kl::entity>();
	test_entity->position.z = 6.0f;
	test_entity->color = (kl::float4) kl::colors::cyan;
	test_entity->mesh = state->meshes["cube"];
	scene->push_back(test_entity);

	state->scene = scene;
}
