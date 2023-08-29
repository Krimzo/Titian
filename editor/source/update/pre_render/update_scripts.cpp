#include "editor.h"


void update_scripts(editor_state* state)
{
	if (state->game_running) {
		state->script_state->update_static_info(state);
		state->script_state->call_updates(state);
	}
}
