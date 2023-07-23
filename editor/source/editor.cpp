#include "editor.h"

#include "setup/setup_state.h"
#include "update/update_state.h"
#include "render/render_chain.h"


int editor_main()
{
    // Setup
    editor_state state = {};
    editor_setup(&state);

    // Loop
    state.timer.reset();
    while (state.window->process(false)) {
        state.timer.update_interval();
        editor_update(&state);
    }
    return 0;
}

void editor_setup(editor_state* state)
{
    state->window->maximize();

    // Render states
    setup_raster_states(state);
    setup_depth_states(state);
    setup_sampler_states(state);

    // Render shaders
    setup_shaders(state);

    // GUI
    setup_gui(state);
}

void editor_update(editor_state* state)
{
    // Pre-Render
    if (state->scene) {
        update_physics(state);

        if (state->scene->camera && state->gui_state->is_viewport_focused) {
            handle_camera(state);
        }
    }

    // Render
    handle_resizes(state);
    render_chain(state);

    // Post-Render
    finalize_frame(state);
}
