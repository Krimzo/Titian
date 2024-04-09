#include "main.h"


titian::GUISectionShaderEditor::GUISectionShaderEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
	: GUISection("GUISectionShaderEditor")
	, editor_layer(editor_layer)
	, gui_layer(gui_layer)
	, m_editor()
{
	m_editor.load_hlsl_standard();
}

void titian::GUISectionShaderEditor::render_gui()
{
	const TimeBomb _ = this->time_it();

	kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
	Scene* scene = &editor_layer->game_layer->scene;

	if (ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2, "ShaderEditorColumns", false);

		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
		if (ImGui::BeginChild("ShaderListChild")) {
			// Create shader
			if (ImGui::BeginPopupContextWindow("NewShader", ImGuiPopupFlags_MouseButtonMiddle)) {
				const std::string name = gui_input_continuous("##CreateShaderInput");
				if (!name.empty()) {
					if (ImGui::MenuItem("New Shader") && !scene->helper_contains_shader(name)) {
						scene->helper_new_shader(name);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}

			// Shaders
			display_shaders(scene);
		}
		ImGui::EndChild();
		ImGui::NextColumn();

		Shader* shader = &scene->get_shader(selected_shader);
		if (shader) {
			edit_shader(shader);

			if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
				const std::filesystem::path path = file.value();
				const std::string extension = path.extension().string();
				if (extension == FILE_EXTENSION_SHADER) {
					shader->data_buffer = kl::read_file_string(path.string());
					shader->reload();
					m_last_shader = nullptr;
				}
			}
		}
		show_shader_properties(shader);
	}
	ImGui::End();
}

void titian::GUISectionShaderEditor::display_shaders(Scene* scene)
{
	const std::string filter = gui_input_continuous("Search###ShaderEditor");
	for (auto& [shader_name, shader] : scene->shaders) {
		if (!filter.empty() && shader_name.find(filter) == -1) {
			continue;
		}

		if (ImGui::Selectable(shader_name.c_str(), shader_name == this->selected_shader)) {
			this->selected_shader = shader_name;
		}

		if (ImGui::BeginPopupContextItem(shader_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			ImGui::Text("Edit Shader");

			if (std::optional opt_name = gui_input_waited("##RenameShaderInput", shader_name)) {
				const std::string& name = opt_name.value();
				if (!name.empty() && !scene->helper_contains_shader(name)) {
					if (this->selected_shader == shader_name) {
						this->selected_shader = name;
					}
					scene->shaders[name] = shader;
					scene->shaders.erase(shader_name);
					should_break = true;
					ImGui::CloseCurrentPopup();
				}
			}

			if (ImGui::Button("Reload", { -1.0f, 0.0f })) {
				shader->reload();
				should_break = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
				if (this->selected_shader == shader_name) {
					this->selected_shader = "/";
				}
				for (auto& [_, material] : scene->materials) {
					if (material->custom_shader_name == shader_name) {
						material->custom_shader_name = "/";
					}
				}
				scene->shaders.erase(shader_name);
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

void titian::GUISectionShaderEditor::show_shader_properties(Shader* shader) const
{
	if (ImGui::Begin("Shader Properties") && shader) {
		ImGui::Text("Info");

		ImGui::Text("Name: ");
		ImGui::SameLine();
		gui_colored_text(selected_shader, gui_layer->special_color);

		ImGui::Text("Type: ");
		ImGui::SameLine();
		ImGui::Text("HLSL");
	}
	ImGui::End();
}

void titian::GUISectionShaderEditor::edit_shader(Shader* shader)
{
	if (shader != m_last_shader) {
		m_last_shader = shader;
		m_editor.load(shader->data_buffer);
	}

	ImGui::PushFont(gui_layer->roboto_font_large);
	m_editor.edit(&shader->data_buffer);
	ImGui::PopFont();
}
