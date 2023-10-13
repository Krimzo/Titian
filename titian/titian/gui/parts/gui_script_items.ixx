export module gui_script_items;

export import gui_render;
export import dll_script;
export import jvm_script;

export void gui_script_items(EditorState* state)
{
	if (ImGui::Begin("Scripts")) {
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Button("Reload all")) {
				for (auto& [_, script] : state->scripts) {
					script->reload();
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		const ImVec2 content_min = ImGui::GetWindowContentRegionMin();
		const ImVec2 content_max = ImGui::GetWindowContentRegionMax();
		const ImVec2 initial_cur_position = ImGui::GetCursorPos();

		ImGui::Dummy({ content_max.x - content_min.x, content_max.y - content_min.y });
		ImGui::SetCursorPos(initial_cur_position);

		const std::optional script_file = gui::drag_drop::read_data<std::string>("ScriptFile");
		if (script_file) {
			const std::filesystem::path path { script_file.value() };
			const std::string script_name = path.stem().string();
			const std::string script_extension = path.extension().string();

			kl::Object<BasicScript> script = nullptr;
			if (script_extension == ".dll") {
				script = new DLLScript(path.string());
			}
			else {
				script = new JVMScript(path.string());
			}
			
			if (script) {
				state->scripts[script_name] = script;
			}
		}

		for (auto& [name, script] : state->scripts) {
			const ImVec4 color = (const ImVec4&) (script->is_valid() ? state->gui_state->color_alternative : state->gui_state->color_special);
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::Selectable(name.c_str(), false);
			ImGui::PopStyleColor();

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Delete")) {
					state->scripts.erase(name);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Reload")) {
					script->reload();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();
}
