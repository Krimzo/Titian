#include "setup/scene/setup_scene.h"


void setup_monkes(state_machine* state, const int size)
{
	const int entity_count = size * size;
	const int half_size = size / 2;

	const float x_offset = -10.0f;

	for (int i = 0; i < entity_count; i++) {
		auto entity = kl::make<kl::entity>();

		entity->scale = kl::float3::splash(0.5f);

		const int x = i % size;
		const int y = i / size;

		entity->position.x = (float) x - half_size + x_offset;
		entity->position.y = (float) y - half_size;
		entity->position.z = 5.0f;

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
