#include "setup/scene/setup_scene.h"


void setup_cubes(state_machine* state, const int size)
{
	const int half_size = size / 2;

	for (int z = 0; z < size; z++) {
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				auto entity = kl::make<kl::entity>();

				entity->scale = kl::float3::splash(0.45f);
				entity->position = kl::float3(x, y, z) - kl::float3::splash(half_size);
				entity->position.z += 5.0f;

				entity->mesh = state->meshes["cube"];
				entity->material.color = (kl::float4)kl::colors::white;

				entity->user_data = new kl::int3{ x - half_size, y - half_size, z - half_size };

				state->scene->push_back(entity);
			}
		}
	}
}
