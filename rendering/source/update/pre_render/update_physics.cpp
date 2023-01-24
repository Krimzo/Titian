#include "update/update_state.h"


void update_physics(state_machine* state)
{
	if (!state->scene) { return; }
	
	const float delta_t = state->timer.get_interval();

	for (auto& entity : *state->scene) {
		entity->update_physics(delta_t);
	}
}
