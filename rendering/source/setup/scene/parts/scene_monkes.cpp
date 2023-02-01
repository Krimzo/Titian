#include "setup/scene/setup_scene.h"


static constexpr float x_offset = -16.0f;

void setup_monkes(state_machine* state, const int size)
{
	const int entity_count = size * size;
	const int half_size = size / 2;

	for (int i = 0; i < entity_count; i++) {
		auto entity = kl::make<kl::entity>();

		entity->set_scale(kl::float3::splash(0.5f));

		const int x = i % size;
		const int y = i / size;

		entity->set_position({
			(x - half_size) + x_offset,
			(y - half_size),
			5.0f
		});

		const float percentage = (i + 1.0f) / entity_count;
		const float normalized = kl::math::minmax(percentage, 0.0f, 1.0f);

		entity->mesh = state->meshes["monke"];
		entity->material.color = kl::float4::splash(normalized);

		state->scene->add(entity);
	}
}
