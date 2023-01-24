#include "setup/setup_state.h"


void fill_with_monkes(state_machine* state, int size);
void fill_with_cubes(state_machine* state, int size);

void setup_scene(state_machine* state)
{
	state->scene = kl::make<kl::scene>();
	fill_with_cubes(state, 3);
	fill_with_monkes(state, 15);
}

void fill_with_monkes(state_machine* state, const int size)
{
	const int entity_count = size * size;
	const int half_size = size / 2;

	for (int i = 0; i < entity_count; i++) {
		auto entity = kl::make<kl::entity>();

		entity->scale = kl::float3::splash(0.5f);

		const int x = i % size;
		const int y = i / size;

		entity->position.x = (float) x - half_size;
		entity->position.y = (float) y - half_size;

		const float percentage = (i + 1.0f) / entity_count;
		const float normalized = kl::math::minmax(percentage, 0.0f, 1.0f);

		entity->mesh = state->meshes["monke"];
		entity->material.color = kl::float4::splash(normalized);

		state->scene->push_back(entity);
	}

	state->scene->sun_direction = { 0, 0, 1 };
	state->scene->camera.position = { 0, 1, -4 };
	state->scene->camera.skybox = state->skyboxes["clouds"];
}

void fill_with_cubes(state_machine* state, const int size)
{
	const int half_size = size / 2;

	for (int z = 0; z < size; z++) {
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				auto entity = kl::make<kl::entity>();

				entity->scale = kl::float3::splash(0.45f);
				entity->position = kl::float3(x, y, z) - kl::float3::splash(half_size);

				entity->mesh = state->meshes["cube"];
				entity->material.color = (kl::float4) kl::colors::red;

				entity->user_data = new kl::int3{ x - half_size, y - half_size, z - half_size };

				state->scene->push_back(entity);
			}
		}
	}

	state->scene->sun_direction = { 1.5f, -1, 2 };
	state->scene->camera.position = { 0, 1, -4 };
	state->scene->camera.skybox = state->skyboxes["clouds"];
}
