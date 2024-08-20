#include "titian.h"


titian::GUISectionScriptingParameters::GUISectionScriptingParameters()
	: GUISection("GUISectionScriptingParameters")
{}

void titian::GUISectionScriptingParameters::render_gui()
{
	const TimeBomb _ = this->time_it();

	GUILayer* gui_layer = Layers::get<GUILayer>();
	Scene* scene = &Layers::get<GameLayer>()->scene;

	Vector<Pair<String, Map<String, Pair<const std::type_info*, void*>>>> scripts;
	scripts.reserve(scene->scripts.size());
	for (auto& [name, script] : scene->scripts) {
		Map<String, Pair<const std::type_info*, void*>> parameters;
		if (InterpScript* interp_script = dynamic_cast<InterpScript*>(&script)) {
			for (auto& [name, value] : interp_script->get_parameters()) {
				parameters[name] = { value.get_type_info().bare_type_info(), value.get_ptr() };
			}
		}
		else if (NodeScript* node_script = dynamic_cast<NodeScript*>(&script)) {
			auto helper = [&]<typename T>(NodeScript::VarStorage<T>& storage)
			{
				for (auto& [name, value] : storage) {
					if (value.first) {
						parameters[name] = { &typeid(T), &value.second };
					}
				}
			};
			helper(node_script->bool_storage);
			helper(node_script->int_storage);
			helper(node_script->int2_storage);
			helper(node_script->float_storage);
			helper(node_script->float2_storage);
			helper(node_script->float3_storage);
			helper(node_script->float4_storage);
			helper(node_script->complex_storage);
			helper(node_script->quaternion_storage);
			helper(node_script->color_storage);
			helper(node_script->string_storage);
		}
		if (!parameters.empty()) {
			scripts.emplace_back(name, parameters);
		}
	}

	im::PushFont(gui_layer->roboto_font_small);

	if (im::Begin("Scripting Parameters")) {
		int script_counter = 1;
		for (auto& [name, parameters] : scripts) {
			if (script_counter > 1) {
				im::Separator();
			}
			im::Text(kl::format("Script ", name).c_str());
			for (auto& [name, pair] : parameters) {
				display_parameter_editor(script_counter, name, *pair.first, pair.second);
			}
			script_counter += 1;
		}
	}
	im::End();

	im::PopFont();
}

void titian::GUISectionScriptingParameters::display_parameter_editor(const int script_id, const String& name, const std::type_info& type, void* ptr)
{
	if (!ptr) {
		return;
	}
	
	if (type == typeid(bool)) {
		const String identifier = kl::format(name, "##", script_id);
		im::Checkbox(identifier.c_str(), (bool*) ptr);
	}
	else if (type == typeid(int)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragInt(identifier.c_str(), (int*) ptr);
	}
	else if (type == typeid(float)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Int2)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragInt2(identifier.c_str(), (int*) ptr);
	}
	else if (type == typeid(Float2)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat2(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Float3)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat3(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Float4)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat4(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Color)) {
		Color* color_ptr = (Color*) ptr;
		Float4 color_as_float = *color_ptr;
		const String identifier = kl::format(name, "##", script_id);
		if (im::ColorEdit4(identifier.c_str(), color_as_float)) {
			*color_ptr = color_as_float;
		}
	}
	else if (type == typeid(char)) {
		char* char_ptr = (char*) ptr;
		char buffer[2] = { *char_ptr, 0 };
		const String identifier = kl::format(name, "##", script_id);
		if (im::InputText(identifier.c_str(), buffer, 2)) {
			*char_ptr = buffer[0];
		}
	}
	else if (type == typeid(String)) {
		const String identifier = kl::format(name, "##", script_id);
		im::InputText(identifier.c_str(), (String*) ptr);
	}
	else {
		const String identifier = kl::format(name, " [unknown]");
		im::Text(identifier.c_str());
	}
}
