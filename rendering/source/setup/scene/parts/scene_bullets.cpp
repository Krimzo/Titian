#include "setup/scene/setup_scene.h"


void setup_bullets(state_machine* state, const int size)
{
	static constexpr float bullet_scale = 10.0f;
	static constexpr float fire_velocity = 2.0f;

	const int half_size = size / 2;
	std::vector<std::function<void()>> all_callbacks = {};

	for (int i = 0; i < size; i++) {
		// Bullet
		kl::ref<kl::entity> bullet = kl::make<kl::entity>();

		bullet->scale *= bullet_scale;
		bullet->rotation = kl::random::get_float3(360.0f);
		bullet->position.x = (float)(i - half_size);

		kl::material bullet_material = {};
		bullet_material.color = (kl::float4)kl::color(232, 230, 227);
		bullet_material.roughness = 0.85f;

		bullet->mesh = state->meshes["bmg_bullet"];
		bullet->material = bullet_material;

		state->scene->push_back(bullet);

		// Bullet casing
		kl::ref<kl::entity> bullet_casing = kl::make<kl::entity>();

		bullet_casing->scale = bullet->scale;
		bullet_casing->rotation = bullet->rotation;
		bullet_casing->position = bullet->position;

		kl::material bullet_casing_material = {};
		bullet_casing_material.color = (kl::float4)kl::color(210, 180, 130);
		bullet_casing_material.roughness = 0.8f;

		bullet_casing->mesh = state->meshes["bmg_casing"];
		bullet_casing->material = bullet_casing_material;

		state->scene->push_back(bullet_casing);

		// Casing primer
		kl::ref<kl::entity> casing_primer = kl::make<kl::entity>();

		casing_primer->scale = bullet_casing->scale;
		casing_primer->rotation = bullet_casing->rotation;
		casing_primer->position = bullet_casing->position;

		kl::material casing_primer_material = {};
		casing_primer_material.color = (kl::float4)kl::color(232, 230, 227);
		casing_primer_material.roughness = 0.9f;

		casing_primer->mesh = state->meshes["bmg_primer"];
		casing_primer->material = casing_primer_material;

		state->scene->push_back(casing_primer);

		// Callbacks
		kl::ref<bool> bullet_fired = kl::make<bool>(false);

		all_callbacks.push_back([state, bullet, casing_primer, bullet_fired]
		{
			if (!*bullet_fired) {
				const kl::float4 default_bullet_direction = { 0.0f, 1.0f, 0.0f, 1.0f };
				auto correct_direction = (kl::mat4::rotation(bullet->rotation) * default_bullet_direction).xyz;

				bullet->velocity = correct_direction.normalize() * fire_velocity;

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
