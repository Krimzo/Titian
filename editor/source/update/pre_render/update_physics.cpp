#include "update/update_state.h"


void update_physics(editor_state* state)
{
    const float delta_t = state->timer.delta();
    state->scene->update_physics(delta_t);
}
