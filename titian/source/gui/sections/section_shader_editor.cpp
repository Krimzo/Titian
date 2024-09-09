#include "titian.h"


titian::GUISectionShaderEditor::GUISectionShaderEditor()
	: GUISection("GUISectionShaderEditor")
{
	m_editor.load_hlsl_standard();
}

void titian::GUISectionShaderEditor::render_gui()
{
	const TimeBomb _ = bench_time_bomb();

	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
	Scene* scene = &Layers::get<GameLayer>()->scene;

	if (im::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoScrollbar)) {
		const float available_width = im::GetContentRegionAvail().x;
		im::Columns(2, "ShaderEditorColumns", false);

		im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
		if (im::BeginChild("ShaderListChild")) {
			if (im::BeginPopupContextWindow("NewShader", ImGuiPopupFlags_MouseButtonMiddle)) {
				const String name = gui_input_continuous("##CreateShaderInput");
				if (!name.empty() && !scene->helper_contains_shader(name)) {
					if (im::MenuItem("New Material Shader")) {
						scene->shaders[name] = new Shader(gpu, ShaderType::MATERIAL);
						im::CloseCurrentPopup();
					}
					if (im::MenuItem("New Camera Shader")) {
						scene->shaders[name] = new Shader(gpu, ShaderType::CAMERA);
						im::CloseCurrentPopup();
					}
				}
				im::EndPopup();
			}
			display_shaders(scene);
		}
		im::EndChild();
		im::NextColumn();

		if (const Optional file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
			if (classify_file(file.value()) == FileType::SHADER) {
				const String name = fs::path(file.value()).filename().string();
				Shader* shader = scene->helper_new_shader(scene->generate_unique_name(name, scene->shaders));
				shader->data_buffer = kl::read_file(file.value());
				shader->reload();
			}
		}

		Shader* shader = scene->helper_get_shader(selected_shader);
		if (shader) {
			edit_shader(shader);
		}
		show_shader_properties(shader);
	}
	im::End();
}

void titian::GUISectionShaderEditor::display_shaders(Scene* scene)
{
	const String filter = gui_input_continuous("Search###ShaderEditor");
	for (auto& [shader_name, shader] : scene->shaders) {
		if (!filter.empty() && shader_name.find(filter) == -1) {
			continue;
		}

		if (shader->shader_type == ShaderType::MATERIAL) {
			im::Button("MATERIAL");
		}
		else if (shader->shader_type == ShaderType::CAMERA) {
			im::Button("CAMERA");
		}
		else {
			im::Button("NONE");
		}
		im::SameLine();

		if (im::Selectable(shader_name.data(), shader_name == this->selected_shader)) {
			this->selected_shader = shader_name;
		}

		if (im::BeginPopupContextItem(shader_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
			bool should_break = false;
			im::Text("Edit Shader");

			if (Optional opt_name = gui_input_waited("##RenameShaderInput", shader_name)) {
				const auto& name = opt_name.value();
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
					im::CloseCurrentPopup();
				}
			}

			if (im::Button("Reload", { -1.0f, 0.0f })) {
				shader->reload();
				should_break = true;
				im::CloseCurrentPopup();
			}

			if (im::Button("Delete", { -1.0f, 0.0f })) {
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
				im::CloseCurrentPopup();
			}

			im::EndPopup();
			if (should_break) {
				break;
			}
		}
	}
}

void titian::GUISectionShaderEditor::show_shader_properties(Shader* shader) const
{
	static const Map<int32_t, String> shader_type_names = {
		{ ShaderType::MATERIAL, "Material" },
		{ ShaderType::CAMERA, "Camera" },
	};

	GUILayer* gui_layer = Layers::get<GUILayer>();

	if (im::Begin("Shader Properties") && shader) {
		im::Text("Info");

		im::Text("Name: ");
		im::SameLine();
		gui_colored_text(selected_shader, gui_layer->special_color);

		if (im::BeginCombo("Shader Type", shader_type_names.at(shader->shader_type).data())) {
			for (auto& [type, name] : shader_type_names) {
				if (im::Selectable(name.data(), shader->shader_type == type)) {
					shader->shader_type = type;
				}
			}
			im::EndCombo();
		}
	}
	im::End();
}

void titian::GUISectionShaderEditor::edit_shader(Shader* shader)
{
	if (shader != m_last_shader) {
		m_last_shader = shader;
		m_editor.load(shader->data_buffer);
	}

	im::PushFont(Layers::get<GUILayer>()->roboto_font_large);
	m_editor.edit(&shader->data_buffer);
	im::PopFont();
}
