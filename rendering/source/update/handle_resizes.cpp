#include "update/update_frame.h"


void handle_resizes(state_machine* state)
{
	static kl::int2 last_size = {};
	const kl::int2 current_size = state->window->size();

	if (current_size.x > 0 && current_size.y > 0 && current_size != last_size) {
		state->gpu->resize_internal(current_size);
		state->gpu->set_viewport(current_size);
		last_size = current_size;
	}
}
