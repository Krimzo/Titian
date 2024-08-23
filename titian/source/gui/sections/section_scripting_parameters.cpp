#include "titian.h"


template<typename V>
static const std::type_info& variant_type(const V& variant)
{
	return std::visit([](auto&& value) -> const std::type_info& { return typeid(value); }, variant);
}

template<typename V>
static void* variant_ptr(V& variant)
{
	return std::visit([](auto&& value) -> void* { return &value; }, variant);
}

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
				parameters[name.substr(2)] = { value.get_type_info().bare_type_info(), value.get_ptr() };
			}
		}
		else if (NodeScript* node_script = dynamic_cast<NodeScript*>(&script)) {
			for (auto& [name, info] : node_script->var_storage) {
				if (info.global) {
					parameters[name] = { &variant_type(info.value), variant_ptr(info.value) };
				}
			}
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
	
	if (type == typeid(void*)) {
		const String identifier = kl::format(name, "##", script_id);
		String ignored = kl::format("0x", *(void**) ptr);
		im::InputText(identifier.c_str(), &ignored, ImGuiInputTextFlags_ReadOnly);
	}
	else if (type == typeid(bool)) {
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
	else if (type == typeid(Float2) || type == typeid(Complex)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat2(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Float3)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat3(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Float4) || type == typeid(Quaternion)) {
		const String identifier = kl::format(name, "##", script_id);
		im::DragFloat4(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(Color)) {
		Color* color_ptr = (Color*) ptr;
		Float4 color_as_float = *color_ptr;
		const String identifier = kl::format(name, "##", script_id);
		if (im::ColorEdit4(identifier.c_str(), &color_as_float.x)) {
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
