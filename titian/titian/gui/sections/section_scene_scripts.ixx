export module section_scene_scripts;

export import gui_section;
export import editor_layer;
export import gui_layer;

export import native_script;
export import interpreted_script;
export import node_script;

export namespace titian {
	class GUISectionSceneScripts : public GUISection
	{
	public:
		EditorLayer* editor_layer = nullptr;
		GUILayer* gui_layer = nullptr;

		GUISectionSceneScripts(EditorLayer* editor_layer, GUILayer* gui_layer)
		{
			this->editor_layer = editor_layer;
			this->gui_layer = gui_layer;
		}

		~GUISectionSceneScripts() override
		{}

		void render_gui() override
		{
			Scene* scene = &editor_layer->game_layer->scene;

			if (ImGui::Begin("Scene Scripts")) {
				if (ImGui::BeginPopupContextWindow()) {
					if (ImGui::Button("Reload all")) {
						for (auto& [_, script] : scene->scripts) {
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

				const std::optional script_file = gui_get_drag_drop<std::string>("ScriptFile");
				if (script_file) {
					const std::filesystem::path path{ script_file.value() };
					const std::string script_name = path.stem().string();
					const std::string script_extension = path.extension().string();

					if (script_extension == ".dll") {
						kl::Object new_script = new NativeScript();
						new_script->data = kl::read_file(path.string());
						scene->scripts[script_name] = new_script;
					}
					else if (script_extension == ".chai") {
						kl::Object new_script = new InterpretedScript();
						new_script->source = kl::read_file_string(path.string());
						scene->scripts[script_name] = new_script;
					}
				}

				const std::string filter = gui_input_continuous("Search###SceneScripts");
				for (auto& [name, script] : scene->scripts) {
					if (!filter.empty() && !name.contains(filter)) {
						continue;
					}

					const ImVec4 color = (const ImVec4&) (script->is_valid() ? gui_layer->alternate_color : gui_layer->special_color);
					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::Selectable(name.c_str(), false);
					ImGui::PopStyleColor();

					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::Button("Delete")) {
							scene->scripts.erase(name);
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
	};
}
