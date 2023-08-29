#pragma once

#include "scripting/engine_script.h"


class dll_script : public engine_script
{
public:
	kl::dll dll = {};

	kl::dll::function<void, editor_state*> start_function = nullptr;
	kl::dll::function<void, editor_state*> update_function = nullptr;

	dll_script();
	dll_script(const std::string& path);

	virtual bool is_valid() const override;
	virtual void reload() override;

	virtual void call_start(editor_state* state) override;
	virtual void call_update(editor_state* state) override;
};
