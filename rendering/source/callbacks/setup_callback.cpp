#include "callbacks/main_callbacks.h"

#include "setup/setup_state.h"


void setup_callback(state_machine* state)
{
	state->window->maximize();

	setup_depth_states(state);
	setup_sampler_states(state);

	setup_shaders(state);
	setup_meshes(state);
	setup_textures(state);
	setup_skyboxes(state);

	setup_scene(state);

	setup_physics(state);
}
