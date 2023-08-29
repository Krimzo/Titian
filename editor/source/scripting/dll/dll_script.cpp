#include "editor.h"


dll_script::dll_script()
{}

dll_script::dll_script(const std::string& path)
	: engine_script(path)
{
	dll.load(path);
	if (dll) {
		start_function = dll.read_function<void, editor_state*>("script_start");
		update_function = dll.read_function<void, editor_state*>("script_update");
	}
}

bool dll_script::is_valid() const
{
	return (dll && start_function && update_function);
}

void dll_script::reload()
{
	dll.reload();
}

void dll_script::call_start(editor_state* state)
{
	if (dll && start_function) {
		start_function(state);
	}
}

void dll_script::call_update(editor_state* state)
{
	if (dll && update_function) {
		update_function(state);
	}
}
