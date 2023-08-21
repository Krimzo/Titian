#include "editor.h"


void gui_script_items(editor_state* state)
{
	if (ImGui::Begin("Scripts")) {
		const ImVec2 content_min = ImGui::GetWindowContentRegionMin();
		const ImVec2 content_max = ImGui::GetWindowContentRegionMax();
		const ImVec2 initial_cur_position = ImGui::GetCursorPos();

		ImGui::Dummy({ content_max.x - content_min.x, content_max.y - content_min.y });
		ImGui::SetCursorPos(initial_cur_position);

		std::optional script_file = GUI::drag_drop::read_data<std::string>("ScriptFile");
		if (script_file) {
			const std::string script_name = std::filesystem::path(script_file.value()).stem().string();
			java::class_data data = java::read_class_data(script_file.value());

			jobject loader = java::new_loader();
			jclass clazz = java::load_class(loader, data);

			kl::object script = new ::script(loader, clazz);
			if (script->valid()) {
				state->script_state->scripts[script_name] = script;
			}
		}

		for (auto& [name, _] : state->script_state->scripts) {
			ImGui::Selectable(name.c_str(), false);
		}
	}
	ImGui::End();
}
