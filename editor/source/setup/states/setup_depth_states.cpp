#include "setup/setup_state.h"


void setup_depth_states(editor_state* state)
{
    state->depth_states["shadow"] = state->gpu->create_depth_state(true, false, false);
    state->depth_states["skybox"] = state->gpu->create_depth_state(false, false, false);
    state->depth_states["entity"] = state->gpu->create_depth_state(true, false, false);
    state->depth_states["collider"] = state->gpu->create_depth_state(false, false, false);
    state->depth_states["postprocess"] = state->gpu->create_depth_state(false, false, false);
}
