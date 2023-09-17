export module editor;

export import editor_state;

export void editor_setup(EditorState* state);
export void editor_update(EditorState* state);

export int editor_main()
{
    // Setup
    EditorState state = {};
    editor_setup(&state);

    // Loop
    state.timer.reset();
    while (state.window->process(false)) {
        state.timer.update_delta();
        editor_update(&state);
    }
    return 0;
}
