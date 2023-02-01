#include "setup/setup_state.h"

#include "setup/scene/setup_scene.h"


static constexpr bool use_ocean = false;

void setup_scene(state_machine* state)
{
	state->scene = kl::make<kl::scene>();
	//state->scene->set_gravity({});

	state->scene->camera.position = { 0, 1, -4 };
	state->scene->sun_direction = { 0.26f, -0.335f, 0.9f };
	state->scene->camera.skybox = state->skyboxes["clouds"];
	
	if constexpr (use_ocean) {
		setup_ocean(state);
	}

	setup_monkes(state, 15);
	setup_spheres(state, 7);
	setup_cubes(state, 15);

	setup_bullets(state, 15);

	setup_physics_test(state, 9);
}
