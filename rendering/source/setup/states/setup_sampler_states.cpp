#include "setup/setup_state.h"


void setup_sampler_states(state_machine* state)
{
	state->sampler_states["skybox"] = state->gpu->new_sampler_state(true, false);
	state->sampler_states["entity"] = state->gpu->new_sampler_state(true, false);
}
