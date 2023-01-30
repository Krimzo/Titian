#include "setup/setup_state.h"

#include "setup/scene/setup_scene.h"


void setup_scene(state_machine* state)
{
	state->scene = kl::make<kl::scene>();
	
	//setup_ocean(state);

	setup_cubes(state, 3);
	setup_spheres(state, 6);

	setup_monkes(state, 15);
	setup_bullets(state, 15);
}
