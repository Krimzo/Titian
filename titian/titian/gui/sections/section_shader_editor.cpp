#include "main.h"


titian::GUISectionShaderEditor::GUISectionShaderEditor(const LayerPackage& package)
	: GUISection("GUISectionShaderEditor", package)
{
	m_editor.load_hlsl_standard();
}

void titian::GUISectionShaderEditor::render_gui()
{
	const TimeBomb _ = this->time_it();

	kl::GPU* gpu = &app_layer->gpu;
	Scene* scene = &game_layer->scene;

	if (ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2, "ShaderEditorColumns", false);

		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
		if (ImGui::BeginChild("ShaderListChild")) {
			// Create shader
			if (ImGui::BeginPopupContextWindow("NewShader", ImGuiPopupFlags_MouseButtonMiddle)) {
				const std::string name = gui_input_continuous("##CreateShaderInput");
				if (!name.empty() && !scene->helper_contains_shader(name)) {
					if (ImGui::MenuItem("New Material Shader")) {
						scene->shaders[name] = new Shader(ShaderType::MATERIAL, gpu);
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::MenuItem("New Camera Shader")) {
						scene->shaders[name] = new Shader(ShaderType::CAMERA, gpu);
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

		// Type
		if (shader->type == ShaderType::MATERIAL) {
			ImGui::Button("MATERIAL");
		}
		else if (shader->type == ShaderType::CAMERA) {
			ImGui::Button("CAMERA");
		}
		else {
			ImGui::Button("NONE");
		}
		ImGui::SameLine();

		if (ImGui::Selectable(shader_name.c_str(), shader_name == this->selected_shader)) {
			this->selected_shader = shader_name;
		}

		if (ImGui::BeginPopupContextItem(shader_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			ImGui::Text("Edit Shader");

			if (std::optional opt_name = gui_input_waited("##RenameShaderInput", shader_name)) {
				const std::string& name = opt_name.value();
				if (!name.empty() && !scene->helper_contains_shader(name)) {
					for (auto& [_, material] : scene->materials) {
						if (material->shader_name == shader_name) {
							material->shader_name = name;
						}
					}
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
					if (material->shader_name == shader_name) {
						material->shader_name = "/";
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

	if (ImGui::Begin("Code Suggestion", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const TextEditor::LanguageDefinition* definition = m_editor.get_definition();
		const std::string current_word = m_editor.get_word_at_cursor();
		if (definition && !current_word.empty()) {
			const TextEditor::Palette& palette = *m_editor.get_palette();
			ImGui::PushStyleColor(ImGuiCol_Text, palette[1]);
			for (const auto& keyword : definition->mKeywords) {
				if (keyword.find(current_word) != -1) {
					if (ImGui::MenuItem(keyword.c_str())) {
						m_editor.replace_word_at_cursor(keyword);
					}
				}
			}
			ImGui::PushStyleColor(ImGuiCol_Text, palette[8]);
			for (const auto& [identifier, _] : definition->mIdentifiers) {
				if (identifier.find(current_word) != -1) {
					if (ImGui::MenuItem(identifier.c_str())) {
						m_editor.replace_word_at_cursor(identifier);
					}
				}
			}
			ImGui::PopStyleColor(2);
		}
	}
	ImGui::End();
	ImGui::PopFont();

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
