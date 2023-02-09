#include "rendering.h"

#include "callbacks/main_callbacks.h"


int rendering_main()
{
    // Setup
    state_machine state = {};
    setup_callback(&state);

    // Loop
    state.timer.reset();
    while (state.window->process(false)) {
        state.timer.update_interval();
        update_callback(&state);
    }
    return 0;
}
