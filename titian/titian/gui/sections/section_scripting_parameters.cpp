#include "main.h"


titian::GUISectionScriptingParameters::GUISectionScriptingParameters(const LayerPackage& package)
	: GUISection("GUISectionScriptingParameters", package)
{}

void titian::GUISectionScriptingParameters::render_gui()
{
	const TimeBomb _ = this->time_it();

	Scene* scene = &game_layer->scene;

	std::vector<std::pair<std::string, std::map<std::string, chaiscript::Boxed_Value>>> scripts;
	scripts.reserve(scene->scripts.size());
	for (auto& [name, script] : scene->scripts) {
		InterpScript* interp_script = dynamic_cast<InterpScript*>(&script);
		if (!interp_script) {
			continue;
		}
		std::map parameters = interp_script->get_parameters();
		if (parameters.empty()) {
			continue;
		}
		scripts.emplace_back(name, parameters);
	}

	imgui::PushFont(gui_layer->roboto_font_small);

	if (imgui::Begin("Scripting Parameters")) {
		int script_counter = 1;
		for (auto& [name, parameters] : scripts) {
			if (script_counter > 1) {
				imgui::Separator();
			}
			imgui::Text(kl::format("Script ", name).c_str());
			for (auto& [name, parameter] : parameters) {
				display_parameter_editor(script_counter, name, parameter);
			}
			script_counter += 1;
		}
	}
	imgui::End();

	imgui::PopFont();
}

void titian::GUISectionScriptingParameters::display_parameter_editor(const int script_id, const std::string& name, const chaiscript::Boxed_Value& parameter)
{
	const chaiscript::Type_Info& type = parameter.get_type_info();
	void* ptr = parameter.get_ptr();
	if (!ptr) {
		return;
	}
	
	if (type == typeid(bool)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::Checkbox(identifier.c_str(), (bool*) ptr);
	}
	else if (type == typeid(int)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::DragInt(identifier.c_str(), (int*) ptr);
	}
	else if (type == typeid(float)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::DragFloat(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Int2)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::DragInt2(identifier.c_str(), (int*) ptr);
	}
	else if (type == typeid(kl::Float2)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::DragFloat2(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Float3)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::DragFloat3(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Float4)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::DragFloat4(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Color)) {
		kl::Color* color_ptr = (kl::Color*) ptr;
		kl::Float4 color_as_float = *color_ptr;
		const std::string identifier = kl::format(name, "##", script_id);
		if (imgui::ColorEdit4(identifier.c_str(), color_as_float)) {
			*color_ptr = color_as_float;
		}
	}
	else if (type == typeid(char)) {
		char* char_ptr = (char*) ptr;
		char buffer[2] = { *char_ptr, 0 };
		const std::string identifier = kl::format(name, "##", script_id);
		if (imgui::InputText(identifier.c_str(), buffer, 2)) {
			*char_ptr = buffer[0];
		}
	}
	else if (type == typeid(std::string)) {
		const std::string identifier = kl::format(name, "##", script_id);
		imgui::InputText(identifier.c_str(), (std::string*) ptr);
	}
	else {
		const std::string identifier = kl::format(name, " [unknown]");
		imgui::Text(identifier.c_str());
	}
}
