#include "setup/setup_state.h"

#include "setup/scene/setup_scene.h"


static constexpr bool use_ocean = false;

void setup_scene(state_machine* state)
{
	state->scene = kl::make<kl::scene>();

	state->scene->camera.far_plane = 75.0f;
	state->scene->camera.position = { 0, 1, -4 };
	state->scene->camera.skybox = state->skyboxes["clouds"];

	state->scene->ambient_light = kl::make<kl::ambient_light>();
	state->scene->directional_light = kl::make<kl::directional_light>(&*state->gpu, 4096);
	state->scene->directional_light->set_direction({ 0.26f, -0.335f, 0.9f });
	
	if constexpr (use_ocean) {
		setup_ocean(state);
	}

	setup_monkes(state, 15);
	setup_spheres(state, 7);
	setup_cubes(state, 15);

	setup_bullets(state, 15);

	setup_physics_test(state, 9);
}
