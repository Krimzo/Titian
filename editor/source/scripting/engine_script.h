#pragma once

#include "klib.h"


struct editor_state;

class engine_script
{
public:
	std::string path = {};

	engine_script();
	engine_script(const std::string& path);

	virtual bool is_valid() const = 0;
	virtual void reload() = 0;

	virtual void call_start(editor_state* state) = 0;
	virtual void call_update(editor_state* state) = 0;
};
