export module editor_setup;

export import editor;
export import jvm_script_transfer;

export void setup_depth_states(editor_state* state);
export void setup_sampler_states(editor_state* state);
export void setup_raster_states(editor_state* state);
export void setup_shaders(editor_state* state);
export void setup_gui(editor_state* state);

export void load_custom_theme(editor_state* state);

export void editor_setup(editor_state* state)
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
