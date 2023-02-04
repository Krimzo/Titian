#include "setup/scene/setup_scene.h"


static constexpr float bullet_scale = 10.0f;
static constexpr float fire_velocity = 2.0f;

void setup_bullets(state_machine* state, const int size)
{
	const int half_size = size / 2;

	std::vector<std::function<void()>> all_callbacks = {};

	for (int i = 0; i < size; i++) {
		// Bullet
		kl::ref<kl::entity> bullet = state->scene->make_entity(true);

		bullet->set_gravity(false);

		bullet->render_scale = bullet->render_scale * bullet_scale;
		bullet->set_rotation(kl::random::get_float3(360.0f));

		kl::float3 old_position = bullet->get_position();
		old_position.x = (float) (i - half_size);
		bullet->set_position(old_position);

		kl::material bullet_material = {};
		bullet_material.color = (kl::float4) kl::color(232, 230, 227);
		bullet_material.reflection_factor = 0.15f;

		bullet->mesh = state->meshes["bmg_bullet"];
		bullet->material = bullet_material;

		state->scene->add(bullet);

		// Bullet casing
		kl::ref<kl::entity> bullet_casing = state->scene->make_entity(false);

		bullet_casing->render_scale = bullet->render_scale;
		bullet_casing->set_rotation(bullet->get_rotation());
		bullet_casing->set_position(bullet->get_position());

		kl::material bullet_casing_material = {};
		bullet_casing_material.color = (kl::float4) kl::color(210, 180, 130);
		bullet_casing_material.reflection_factor = 0.2f;

		bullet_casing->mesh = state->meshes["bmg_casing"];
		bullet_casing->material = bullet_casing_material;

		state->scene->add(bullet_casing);

		// Casing primer
		kl::ref<kl::entity> casing_primer = state->scene->make_entity(false);

		casing_primer->render_scale = bullet_casing->render_scale;
		casing_primer->set_rotation(bullet_casing->get_rotation());
		casing_primer->set_position(bullet_casing->get_position());

		kl::material casing_primer_material = {};
		casing_primer_material.color = (kl::float4) kl::color(232, 230, 227);
		casing_primer_material.reflection_factor = 0.1f;

		casing_primer->mesh = state->meshes["bmg_primer"];
		casing_primer->material = casing_primer_material;

		state->scene->add(casing_primer);

		// Callbacks
		kl::ref<bool> bullet_fired = kl::make<bool>(false);

		all_callbacks.push_back([state, bullet, casing_primer, bullet_fired]
		{
			if (!*bullet_fired) {
				const kl::float4 default_bullet_direction = { 0.0f, 1.0f, 0.0f, 1.0f };
				auto correct_direction = (kl::mat4::rotation(bullet->get_rotation()) * default_bullet_direction).xyz;

				bullet->set_mass(1.0f);
				bullet->set_velocity(correct_direction.normalize() * fire_velocity);

				casing_primer->mesh = state->meshes["bmg_primer_fired"];

				*bullet_fired = true;
			}
		});
	}

	state->window->keyboard.space.on_release = [all_callbacks]
	{
		for (auto& callback : all_callbacks) {
			callback();
		}
	};
}
