#pragma once

#include "scripting/engine_script.h"
#include "jdk.h"


class jvm_script : public engine_script
{
public:
	java::holder<jobject> loader { nullptr };
	java::holder<jclass> clazz { nullptr };
	java::holder<jobject> instance { nullptr };

	jmethod start_method = {};
	jmethod update_method = {};

	jvm_script();
	jvm_script(const std::string& path);

	virtual bool is_valid() const override;
	virtual void reload() override;

	virtual void call_start(editor_state* state) override;
	virtual void call_update(editor_state* state) override;
};
