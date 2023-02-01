#include "setup/scene/setup_scene.h"


void setup_platform(state_machine* state);
void setup_objects(state_machine* state, int size);

void setup_physics_test(state_machine* state, const int size)
{
	setup_platform(state);
	setup_objects(state, size);
}

void setup_platform(state_machine* state)
{
	kl::ref<kl::entity> platform = kl::make<kl::entity>();

	platform->set_mass(0.0f);
	platform->set_collider(kl::make_box_collider());

	platform->set_scale({ 15.0f, 0.1f, 15.0f });
	platform->set_position({ 0.0f, -7.0f, -25.0f });

	platform->mesh = state->meshes["cube"];
	platform->material.color = kl::float4::splash(1.0f);

	state->scene->add(platform);
}

void setup_objects(state_machine* state, const int size)
{
	const int half_size = size / 2;

	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			kl::ref<kl::entity> box = kl::make<kl::entity>();

			box->set_mass(2.5f);
			box->set_collider(kl::make_box_collider());

			box->set_scale({ 0.5f, 0.5f, 0.5f });
			box->set_rotation(kl::random::get_float3(360.0f));
			box->set_position({ (half_size - x) * 2.25f, 15.0f, -25.0f + (half_size - z) * 2.25f });

			box->mesh = state->meshes["cube"];
			box->material.color = (kl::float4)kl::random::get_color();

			state->scene->add(box);
		}
	}
}
