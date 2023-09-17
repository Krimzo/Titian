export module editor_setup;

export import editor;
export import jvm_script_transfer;

export void setup_depth_states(EditorState* state);
export void setup_sampler_states(EditorState* state);
export void setup_raster_states(EditorState* state);
export void setup_shaders(EditorState* state);
export void setup_gui(EditorState* state);

export void load_custom_theme(EditorState* state);

export void editor_setup(EditorState* state)
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

    // Scripting
    script_transfer::bind_state(state);
    script_transfer::bind_native_methods();
    script_transfer::load_classes();
}
