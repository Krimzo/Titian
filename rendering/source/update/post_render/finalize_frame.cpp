#include "update/update_state.h"


static constexpr int update_interval = 10;

void finalize_frame(state_machine* state)
{
    static size_t counter = 0;
    counter += 1;

    if (counter % update_interval == 0) {
        const int fps = (int) (1.0f / state->timer.get_interval());
        std::string title = kl::format("[", fps, "]");
        state->window->set_title(title);
    }
}
