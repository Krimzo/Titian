#pragma once

#include "scripting/engine_script.h"


struct editor_state;

struct script_state
{
	std::map<std::string, kl::object<engine_script>> scripts = {};

	script_state();

	void update_static_info(editor_state* state);

	void call_starts(editor_state* state);
	void call_updates(editor_state* state);
};
