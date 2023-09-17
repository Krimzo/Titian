export module setup_depth_states;

export import editor_setup;

export void setup_depth_states(EditorState* state)
{
    state->depth_states.disabled = state->gpu->create_depth_state(false);
    state->depth_states.enabled = state->gpu->create_depth_state(true);
}
