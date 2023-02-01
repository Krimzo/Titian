#include "setup/scene/setup_scene.h"


static constexpr float x_offset = 0.0f;

void setup_spheres(state_machine* state, const int size)
{
	const int half_size = size / 2;

	int sphere_counter = 0;

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			kl::ref<kl::entity> sphere = kl::make<kl::entity>();

			sphere->set_position({
				(x - half_size) * 2.25f + x_offset,
				(y - half_size) * 2.25f,
				5.0f
			});

			sphere->mesh = state->meshes["sphere"];

			sphere->material.color = kl::float4::splash(1.0f);
			sphere->material.reflection_factor = (float) ++sphere_counter / (size * size);

			state->scene->add(sphere);
		}
	}
}
