#include "editor.h"


script::script()
{}

script::script(jobject loader, jclass clazz)
	: loader(loader), clazz(clazz)
{
	if (!clazz) {
		return;
	}

	jmethod constructor = java::get_constructor(clazz);
	if (!constructor) {
		return;
	}

	jobject object = java::new_object(clazz, constructor);
	if (!object) {
		return;
	}

	instance.set(object);
	start_method = java::get_method(clazz, "start");
	update_method = java::get_method(clazz, "update");
}

bool script::valid() const
{
	return (instance && start_method && update_method);
}

bool script::call_start() const
{
	if (!instance || !start_method) {
		return false;
	}
	java::jni->CallVoidMethod(instance.get(), start_method);
	return true;
}

bool script::call_update() const
{
	if (!instance || !start_method) {
		return false;
	}
	java::jni->CallVoidMethod(instance.get(), update_method);
	return true;
}
