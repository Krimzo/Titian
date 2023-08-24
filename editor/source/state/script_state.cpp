#include "editor.h"


script_state::script_state()
{}

void script_state::update_static_info(editor_state* state)
{
	static jclass class_engine = java::load_eternal_class("Lsdk/engine/Engine;");
	if (!class_engine) {
		return;
	}

	static jfield field_elapsed_t = java::jni->GetStaticFieldID(class_engine, "elapsedTime", signatures::FLOAT.c_str());
	if (!field_elapsed_t) {
		return;
	}

	static jfield field_delta_t = java::jni->GetStaticFieldID(class_engine, "deltaTime", signatures::FLOAT.c_str());
	if (!field_delta_t) {
		return;
	}

	const float elapsed_t = state->timer.elapsed();
	const float delta_t = state->timer.delta();

	java::jni->SetStaticFloatField(class_engine, field_elapsed_t, elapsed_t);
	java::jni->SetStaticFloatField(class_engine, field_delta_t, delta_t);
}

void script_state::call_starts()
{
	for (const auto& [_, script] : scripts) {
		script->call_start();
	}
}

void script_state::call_updates()
{
	for (const auto& [_, script] : scripts) {
		script->call_update();
	}
}
