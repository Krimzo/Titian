#include "main.h"


titian::GUISectionScriptEditor::GUISectionScriptEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
{
	this->editor_layer = editor_layer;
	this->gui_layer = gui_layer;
}

void titian::GUISectionScriptEditor::render_gui()
{
	kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
	Scene* scene = &editor_layer->game_layer->scene;

	if (ImGui::Begin("Script Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2, "ScriptEditorColumns", false);

		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
		if (ImGui::BeginChild("ScriptListChild")) {
			// Create script
			if (ImGui::BeginPopupContextWindow("NewScript", ImGuiPopupFlags_MouseButtonMiddle)) {
				const std::string name = gui_input_continuous("##CreateScriptInput");
				if (!name.empty()) {
					if (ImGui::MenuItem("New Native Script") && !scene->scripts.contains(name)) {
						scene->scripts[name] = new NativeScript();
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::MenuItem("New Inter Script") && !scene->scripts.contains(name)) {
						scene->scripts[name] = new InterScript();
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}

			// Scripts
			display_scripts(scene);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

		Script* script = &scene->get_script(selected_script);
		NativeScript* native_script = dynamic_cast<NativeScript*>(script);
		InterScript* inter_script = dynamic_cast<InterScript*>(script);

		if (ImGui::BeginChild("ScriptEditorChild")) {
			if (native_script) {
				edit_native_script(native_script);

				if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
					const std::filesystem::path path = file.value();
					const std::string extension = path.extension().string();
					if (extension == FILE_EXTENSION_NATIVE_SCRIPT) {
						native_script->data = kl::read_file(path.string());
						native_script->reload();
					}
				}
			}
			else if (inter_script) {
				edit_inter_script(inter_script);

				if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
					const std::filesystem::path path = file.value();
					const std::string extension = path.extension().string();
					if (extension == FILE_EXTENSION_INTER_SCRIPT) {
						inter_script->source = kl::read_file_string(path.string());
						inter_script->reload();
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::PopStyleVar(2);

		show_script_properties(script);
	}
	ImGui::End();
}

void titian::GUISectionScriptEditor::display_scripts(Scene* scene)
{
	const std::string filter = gui_input_continuous("Search###ScriptEditor");
	for (auto& [script_name, script] : scene->scripts) {
		if (!filter.empty() && script_name.find(filter) == -1) {
			continue;
		}

		// Script type
		if (dynamic_cast<const NativeScript*>(&script)) {
			ImGui::Button("NATIVE");
		}
		else if (dynamic_cast<const InterScript*>(&script)) {
			ImGui::Button("INTER");
		}
		else {
			ImGui::Button("SCRIPT");
		}
		ImGui::SameLine();

		// Script name
		if (ImGui::Selectable(script_name.c_str(), script_name == this->selected_script)) {
			this->selected_script = script_name;
		}

		if (ImGui::BeginPopupContextItem(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			ImGui::Text("Edit Script");

			if (std::optional opt_name = gui_input_waited("##RenameScriptInput", script_name)) {
				const std::string& name = opt_name.value();
				if (!name.empty() && !scene->scripts.contains(name)) {
					if (this->selected_script == script_name) {
						this->selected_script = name;
					}
					scene->scripts[name] = script;
					scene->scripts.erase(script_name);
					should_break = true;
					ImGui::CloseCurrentPopup();
				}
			}

			if (ImGui::Button("Reload", { -1.0f, 0.0f })) {
				script->reload();
				should_break = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
				if (this->selected_script == script_name) {
					this->selected_script = "/";
				}
				scene->scripts.erase(script_name);
				should_break = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
			if (should_break) {
				break;
			}
		}
	}
}

void titian::GUISectionScriptEditor::show_script_properties(Script* script) const
{
	if (ImGui::Begin("Script Properties") && script) {
		ImGui::Text("Info");

		ImGui::Text("Name: ");
		ImGui::SameLine();
		gui_colored_text(selected_script, gui_layer->special_color);

		ImGui::Text("Type: ");
		ImGui::SameLine();
		if (NativeScript* native_script = dynamic_cast<NativeScript*>(script)) {
			ImGui::Text("NATIVE");
		}
		else if (InterScript* inter_script = dynamic_cast<InterScript*>(script)) {
			ImGui::Text("INTER");
		}
	}
	ImGui::End();
}

void titian::GUISectionScriptEditor::edit_native_script(NativeScript* script)
{
	std::vector<byte>& data = script->data;
	m_memory_editor.DrawContents(data.data(), data.size());
}

void titian::GUISectionScriptEditor::edit_inter_script(InterScript* script)
{
	ImGui::PushFont(gui_layer->jetbrains_font);
	const std::string id = kl::format("##", selected_script);
	ImGui::InputTextMultiline(id.c_str(), &script->source, ImVec2(-1.0f, -1.0f), ImGuiInputTextFlags_AllowTabInput);
	ImGui::PopFont();
}
