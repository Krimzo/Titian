#include "render/render_chain.h"


static kl::int2 last_window_size = {};

void handle_resizes(state_machine* state)
{
    const kl::int2 current_window_size = state->window->size();

    if (current_window_size.x > 0 && current_window_size.y > 0 && current_window_size != last_window_size) {
        state->gpu->resize_internal(current_window_size);
        state->gpu->set_viewport(current_window_size);
        last_window_size = current_window_size;
    }
}
