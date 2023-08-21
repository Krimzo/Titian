#pragma once

#include "scripting/script.h"


struct editor_state;

struct script_state
{
	std::map<std::string, kl::object<script>> scripts = {};

	script_state();

	void update_static_info(editor_state* state);

	void call_starts();
	void call_updates();
};
