#include "setup/setup_state.h"


void setup_depth_states(state_machine* state)
{
    state->depth_states["shadow"] = state->gpu->new_depth_state(true, false, false);

    state->depth_states["skybox"] = state->gpu->new_depth_state(false, false, false);
    state->depth_states["ocean"] = state->gpu->new_depth_state(true, false, false);

    state->depth_states["entity"] = state->gpu->new_depth_state(true, false, false);
    state->depth_states["postprocess"] = state->gpu->new_depth_state(false, false, false);
}
