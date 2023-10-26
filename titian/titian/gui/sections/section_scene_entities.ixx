export module section_scene_entities;

export import gui_section;
export import editor_layer;

export namespace titian {
	class GUISectionSceneEntities : public GUISection
	{
	public:
		kl::Object<EditorLayer> editor_layer = nullptr;

		GUISectionSceneEntities(kl::Object<EditorLayer>& editor_layer)
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
				for (auto& [name, _] : *scene) {
					if (!filter.empty() && !name.contains(filter)) {
						continue;
					}
					if (ImGui::Selectable(name.c_str(), name == editor_layer->selected_entity)) {
						editor_layer->selected_entity = name;
					}
				}
			}
			ImGui::End();
		}
	};
}
