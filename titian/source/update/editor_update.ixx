export module editor_update;

export import editor;

export void update_physics(EditorState* state);
export void update_scripts(EditorState* state);
export void handle_camera(EditorState* state);
export void finalize_frame(EditorState* state);

export void handle_resizes(EditorState* state);
export void render_chain(EditorState* state);

export void editor_update(EditorState* state)
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
