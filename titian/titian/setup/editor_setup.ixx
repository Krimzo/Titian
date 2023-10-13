export module editor_setup;

export void editor_setup(EditorState* state)
{
    script_transfer::bind_state(state);
    script_transfer::bind_native_methods();
    script_transfer::load_classes();
}
