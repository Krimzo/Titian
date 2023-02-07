#include "rendering.h"

#include "callbacks/main_callbacks.h"


int rendering_main()
{
    // Setup
    state_machine state = {};
    state.window = kl::make<kl::window>(kl::int2(1600, 900), "Fun");
    state.gpu = kl::make<kl::gpu>(state.window->get_window());
    state.scene = kl::make<kl::scene>();
    setup_callback(&state);

    // Loop
    state.timer.reset();
    while (state.window->process(false)) {
        state.timer.update_interval();
        update_callback(&state);
    }

    // Cleanup
    state.meshes.clear();
    state.materials.clear();
    state.scene = nullptr;
    return 0;
}
