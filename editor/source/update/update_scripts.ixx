export module update_scripts;

export import editor_update;

export void update_scripts(EditorState* state)
{
	if (state->game_running) {
		state->scripts_update_static_info(state);
		state->scripts_call_update(state);
	}
}
