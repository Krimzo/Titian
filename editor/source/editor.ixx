export module editor;

export import editor_state;

export void editor_setup(editor_state* state);
export void editor_update(editor_state* state);

export int editor_main()
{
    // Setup
    editor_state state = {};
    editor_setup(&state);

    // Loop
    state.timer.reset();
    while (state.window->process(false)) {
        state.timer.update_delta();
        editor_update(&state);
    }
    return 0;
}
