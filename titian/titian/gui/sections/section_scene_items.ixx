export module section_scene_items;

export import gui_section;
export import editor_layer;

export namespace titian {
	class GUISectionSceneItems : public GUISection
	{
	public:
		kl::Object<EditorLayer> editor_layer = nullptr;

		GUISectionSceneItems(kl::Object<EditorLayer>& editor_layer)
		{
			this->editor_layer = editor_layer;
		}

		~GUISectionSceneItems() override
		{}

		void render_gui() override
		{
			Scene& scene = *editor_layer->game_layer->scene;

			if (ImGui::Begin("Scene items")) {
				for (auto& [name, entity] : scene) {
					if (ImGui::Selectable(name.c_str(), entity == editor_layer->selected_entity)) {
						editor_layer->selected_entity = entity;
					}
				}
			}
			ImGui::End();
		}
	};
}
