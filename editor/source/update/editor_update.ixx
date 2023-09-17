export module editor_update;

export import editor;

export void update_physics(editor_state* state);
export void update_scripts(editor_state* state);
export void handle_camera(editor_state* state);
export void finalize_frame(editor_state* state);

export void handle_resizes(editor_state* state);
export void render_chain(editor_state* state);

export void editor_update(editor_state* state)
{
    // Pre-Render
    if (state->scene) {
        update_physics(state);
        update_scripts(state);

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
