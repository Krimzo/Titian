#include "setup/setup_state.h"

void setup_ocean(state_machine* state);
void fill_with_monkes(state_machine* state, int size);
void fill_with_cubes(state_machine* state, int size);
void fill_with_bullets(state_machine* state, int size);


void setup_scene(state_machine* state)
{
	state->scene = kl::make<kl::scene>();
	
	//setup_ocean(state);

	fill_with_cubes(state, 3);
	fill_with_monkes(state, 15);
	fill_with_bullets(state, 15);
}

void setup_ocean(state_machine* state)
{
	static constexpr float size_in_meters = 100.0f;
	static constexpr float points_per_meter = 15.0f;
	state->scene->ocean_mesh = state->gpu->generate_plane_mesh(size_in_meters, (int) (size_in_meters * points_per_meter));
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
		entity->position.z = 10.0f;

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
				entity->position.z -= 10.0f;

				entity->mesh = state->meshes["cube"];
				entity->material.color = (kl::float4) kl::colors::white;

				entity->user_data = new kl::int3{ x - half_size, y - half_size, z - half_size };

				state->scene->push_back(entity);
			}
		}
	}

	state->scene->sun_direction = { 1.5f, -1, 2 };
	state->scene->camera.position = { 0, 1, -4 };
	state->scene->camera.skybox = state->skyboxes["clouds"];
}

void fill_with_bullets(state_machine* state, const int size)
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
		bullet->position.x = (float) (i - half_size);

		kl::material bullet_material = {};
		bullet_material.color = (kl::float4) kl::color(232, 230, 227);

		bullet->mesh = state->meshes["bmg_bullet"];
		bullet->material = bullet_material;

		state->scene->push_back(bullet);

		// Bullet casing
		kl::ref<kl::entity> bullet_casing = kl::make<kl::entity>();

		bullet_casing->scale = bullet->scale;
		bullet_casing->rotation = bullet->rotation;
		bullet_casing->position = bullet->position;

		kl::material bullet_casing_material = {};
		bullet_casing_material.color = (kl::float4) kl::color(210, 180, 130);

		bullet_casing->mesh = state->meshes["bmg_casing"];
		bullet_casing->material = bullet_casing_material;

		state->scene->push_back(bullet_casing);

		// Casing primer
		kl::ref<kl::entity> casing_primer = kl::make<kl::entity>();

		casing_primer->scale = bullet_casing->scale;
		casing_primer->rotation = bullet_casing->rotation;
		casing_primer->position = bullet_casing->position;

		kl::material casing_primer_material = {};
		casing_primer_material.color = (kl::float4) kl::color(232, 230, 227);

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
