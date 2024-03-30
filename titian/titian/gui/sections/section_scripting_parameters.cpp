#include "main.h"


titian::GUISectionScriptingParameters::GUISectionScriptingParameters(GameLayer* game_layer, GUILayer* gui_layer)
	: game_layer(game_layer), gui_layer(gui_layer)
{}

void titian::GUISectionScriptingParameters::render_gui()
{
	Scene* scene = &game_layer->scene;

	std::vector<std::pair<std::string, std::map<std::string, chaiscript::Boxed_Value>>> scripts;
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

	ImGui::PushFont(gui_layer->jetbrains_font_small);

	if (ImGui::Begin("Scripting Parameters")) {
		if (!scripts.empty()) {
			ImGui::Separator();
		}
		int script_counter = 0;
		for (auto& [name, parameters] : scripts) {
			ImGui::Text(kl::format("Interp Script: ", name).c_str());
			for (auto& [name, parameter] : parameters) {
				display_parameter_editor(++script_counter, name, parameter);
			}
			ImGui::Separator();
		}
	}
	ImGui::End();

	ImGui::PopFont();
}

void titian::GUISectionScriptingParameters::display_parameter_editor(const int script_id, const std::string& name, const chaiscript::Boxed_Value& parameter)
{
	const std::string name_part = kl::format("(", name, ")");
	const chaiscript::Type_Info& type = parameter.get_type_info();
	void* ptr = parameter.get_ptr();
	
	if (type == typeid(bool)) {
		const std::string identifier = kl::format(name_part, " [bool] ##", script_id);
		ImGui::Checkbox(identifier.c_str(), (bool*) ptr);
	}
	else if (type == typeid(int)) {
		const std::string identifier = kl::format(name_part, " [int] ##", script_id);
		ImGui::DragInt(identifier.c_str(), (int*) ptr);
	}
	else if (type == typeid(float)) {
		const std::string identifier = kl::format(name_part, " [float] ##", script_id);
		ImGui::DragFloat(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Int2)) {
		const std::string identifier = kl::format(name_part, " [Int2] ##", script_id);
		ImGui::DragInt2(identifier.c_str(), (int*) ptr);
	}
	else if (type == typeid(kl::Float2)) {
		const std::string identifier = kl::format(name_part, " [Float2] ##", script_id);
		ImGui::DragFloat2(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Float3)) {
		const std::string identifier = kl::format(name_part, " [Float3] ##", script_id);
		ImGui::DragFloat3(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Float4)) {
		const std::string identifier = kl::format(name_part, " [Float4] ##", script_id);
		ImGui::DragFloat4(identifier.c_str(), (float*) ptr);
	}
	else if (type == typeid(kl::Color)) {
		kl::Color* color_ptr = (kl::Color*) ptr;
		kl::Float4 color_as_float = *color_ptr;
		const std::string identifier = kl::format(name_part, " [Color] ##", script_id);
		if (ImGui::ColorEdit4(identifier.c_str(), color_as_float)) {
			*color_ptr = color_as_float;
		}
	}
	else if (type == typeid(char)) {
		char* char_ptr = (char*) ptr;
		char buffer[2] = { *char_ptr, 0 };
		const std::string identifier = kl::format(name_part, " [char] ##", script_id);
		if (ImGui::InputText(identifier.c_str(), buffer, 2)) {
			*char_ptr = buffer[0];
		}
	}
	else if (type == typeid(std::string)) {
		const std::string identifier = kl::format(name_part, " [string] ##", script_id);
		ImGui::InputText(identifier.c_str(), (std::string*) ptr);
	}
	else {
		const std::string identifier = kl::format(name_part, " [unknown]");
		ImGui::Text(identifier.c_str());
	}
}
