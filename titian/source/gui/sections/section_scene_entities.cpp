#include "titian.h"


titian::GUISectionSceneEntities::GUISectionSceneEntities()
	: GUISection("GUISectionSceneEntities")
{}

void titian::GUISectionSceneEntities::render_gui()
{
	const TimeBomb _ = bench_time_bomb();

	EditorLayer* editor_layer = Layers::get<EditorLayer>();
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
	Scene* scene = &Layers::get<GameLayer>()->scene;

	if (im::Begin("Scene Entities")) {
		// Window popup
		if (im::BeginPopupContextWindow("NewEntity", ImGuiPopupFlags_MouseButtonMiddle)) {
			const String name = gui_input_continuous("##CreateEntityInput");
			if (!name.empty() && !scene->helper_contains_entity(name)) {
				if (im::MenuItem("New Entity")) {
					Ref entity = scene->new_entity(false);
					scene->add_entity(name, entity);
					im::CloseCurrentPopup();
				}
				if (im::MenuItem("New Camera")) {
					Ref entity = scene->new_casted<Camera>(false);
					scene->add_entity(name, entity);
					im::CloseCurrentPopup();
				}
				if (im::MenuItem("New Ambient Light")) {
					Ref entity = scene->new_casted<AmbientLight>(false);
					scene->add_entity(name, entity);
					im::CloseCurrentPopup();
				}
				if (im::MenuItem("New Point Light")) {
					Ref entity = scene->new_casted<PointLight>(false);
					scene->add_entity(name, entity);
					im::CloseCurrentPopup();
				}
				if (im::MenuItem("New Directional Light")) {
					Ref entity = scene->new_casted<DirectionalLight>(false, gpu);
					scene->add_entity(name, entity);
					im::CloseCurrentPopup();
				}
			}
			im::EndPopup();
		}

		if (im::IsWindowFocused() && im::IsMouseHoveringRect(im::GetWindowPos(), im::GetWindowPos() + im::GetWindowSize())) {
			if (im::IsKeyPressed(ImGuiKey_Delete)) {
				for (auto& name : editor_layer->selected_entities) {
					scene->remove_entity(name);
				}
				editor_layer->selected_entities.clear();
			}
		}

		// Entities
		const String filter = gui_input_continuous("Search###SceneEntities");
		for (auto& [entity_name, entity] : scene->entities()) {
			if (!filter.empty() && entity_name.find(filter) == -1) {
				continue;
			}

			// Entity type
			if (entity.is<Camera>()) {
				im::Button("CAMERA");
			}
			else if (entity.is<AmbientLight>()) {
				im::Button("AMBIENT");
			}
			else if (entity.is<PointLight>()) {
				im::Button("POINT");
			}
			else if (entity.is<DirectionalLight>()) {
				im::Button("DIRECTIONAL");
			}
			else {
				im::Button("ENTITY");
			}
			im::SameLine();

			// Entity name
			if (im::Selectable(entity_name.data(), editor_layer->selected_entities.contains(entity_name))) {
				if (im::IsKeyDown(ImGuiKey_LeftCtrl)) {
					if (editor_layer->selected_entities.contains(entity_name)) {
						editor_layer->selected_entities.erase(entity_name);
					}
					else {
						editor_layer->selected_entities.insert(entity_name);
					}
				}
				else if (im::IsKeyDown(ImGuiKey_LeftShift)) {
					if (editor_layer->selected_entities.empty()) {
						editor_layer->selected_entities.insert(entity_name);
					}
					else {
						int find_counter = 0;
						Vector<String> needed_entities;
						for (const auto& [name, _] : scene->entities()) {
							if (name == entity_name || name == *--editor_layer->selected_entities.end()) {
								find_counter += 1;
							}
							if (find_counter > 0) {
								needed_entities.push_back(name);
							}
							if (find_counter >= 2) {
								break;
							}
						}
						editor_layer->selected_entities.insert(needed_entities.begin(), needed_entities.end());
					}
				}
				else {
					editor_layer->selected_entities = { entity_name };
				}
			}

			// Item popup
			if (im::BeginPopupContextItem(entity_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
				bool should_break = false;
				im::Text("Edit Entity");

				if (Optional opt_name = gui_input_waited("##RenameEntityInput", entity_name)) {
					const auto& name = opt_name.value();
					if (!name.empty() && !scene->helper_contains_entity(name)) {
						Ref temp_holder = entity;
						scene->remove_entity(entity_name);
						scene->add_entity(name, temp_holder);

						should_break = true;
						im::CloseCurrentPopup();
					}
				}

				if (im::Button("Delete", { -1.0f, 0.0f })) {
					scene->remove_entity(entity_name);

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
	im::End();
}
