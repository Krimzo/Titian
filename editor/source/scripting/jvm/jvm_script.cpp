#include "editor.h"


jvm_script::jvm_script()
{}

jvm_script::jvm_script(const std::string& path)
	: engine_script(path)
{
	reload();
}

bool jvm_script::is_valid() const
{
	return (instance && start_method && update_method);
}

void jvm_script::reload()
{
	// Reset
	update_method = nullptr;
	start_method = nullptr;
	instance.set(nullptr);
	loader.set(nullptr);
	clazz.set(nullptr);

	// Load
	loader.set(java::new_loader());
	clazz.set(java::load_class(loader.get(), java::read_class_data(path)));

	jmethod constructor = java::get_constructor(clazz.get());
	if (!constructor) {
		return;
	}
	jobject object = java::new_object(clazz.get(), constructor);
	if (!object) {
		return;
	}

	instance.set(object);
	start_method = java::get_method(clazz.get(), "start");
	update_method = java::get_method(clazz.get(), "update");
}

void jvm_script::call_start(editor_state* state)
{
	if (instance && start_method) {
		java::jni->CallVoidMethod(instance.get(), start_method);
	}
}

void jvm_script::call_update(editor_state* state)
{
	if (instance || start_method) {
		java::jni->CallVoidMethod(instance.get(), update_method);
	}
}
