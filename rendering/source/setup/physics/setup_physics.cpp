#include "setup/setup_state.h"


void setup_physics(state_machine* state)
{
	state->physics_scene = kl::make<physics_scene>();
}
