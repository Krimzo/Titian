#include "editor.h"


void setup_depth_states(editor_state* state)
{
    state->depth_states.disabled = state->gpu->create_depth_state(false);
    state->depth_states.enabled = state->gpu->create_depth_state(true);
}
