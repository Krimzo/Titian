#include "callbacks/main_callbacks.h"

#include "update/update_state.h"
#include "render/render_chain.h"


void update_callback(state_machine* state)
{
    // Pre-Render
    if (state->scene) {
        update_physics(state);
        update_light(state);
        handle_camera(state);
    }

    // Custom-Pre-Render
    custom_pre_render_update(state);

    // Render
    handle_resizes(state);
    render_chain(state);

    // Post-Render
    finalize_frame(state);

    // Custom-Post-Render
    custom_post_render_update(state);
}
