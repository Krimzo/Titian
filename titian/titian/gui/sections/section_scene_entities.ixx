export module section_scene_entities;

export import gui_section;
export import editor_layer;

export namespace titian {
	class GUISectionSceneEntities : public GUISection
	{
	public:
		EditorLayer* editor_layer = nullptr;

		GUISectionSceneEntities(EditorLayer* editor_layer)
		{
			this->editor_layer = editor_layer;
		}

		~GUISectionSceneEntities() override
		{}

		void render_gui() override
		{
			Scene* scene = &editor_layer->game_layer->scene;

			if (ImGui::Begin("Scene Entities")) {
				const std::string filter = gui_input_continuous("Search###SceneEntities");
				for (auto& [entity_name, entity] : *scene) {
					if (!filter.empty() && !entity_name.contains(filter)) {
						continue;
					}
					if (ImGui::Selectable(entity_name.c_str(), entity_name == editor_layer->selected_entity)) {
						editor_layer->selected_entity = entity_name;
					}

					if (ImGui::BeginPopupContextItem(entity_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
						bool should_break = false;
						ImGui::Text("Edit Entity");

						if (std::optional name = gui_input_waited("##RenameEntityInput", entity_name)) {
							if (!scene->contains(name.value())) {
								kl::Object temp_holder = entity;
								scene->remove(entity_name);
								scene->add(name.value(), temp_holder);

								should_break = true;
								ImGui::CloseCurrentPopup();
							}
						}

						if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
							scene->remove(entity_name);

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
			ImGui::End();
		}
	};
}
