#include "main.h"


titian::GUISectionSceneEntities::GUISectionSceneEntities(const LayerPackage& package)
	: GUISection("GUISectionSceneEntities", package)
{}

void titian::GUISectionSceneEntities::render_gui()
{
	const TimeBomb _ = this->time_it();

	kl::GPU* gpu = &app_layer->gpu;
	Scene* scene = &game_layer->scene;

	if (imgui::Begin("Scene Entities")) {
		// Window popup
		if (imgui::BeginPopupContextWindow("NewEntity", ImGuiPopupFlags_MouseButtonMiddle)) {
			const std::string name = gui_input_continuous("##CreateEntityInput");
			if (!name.empty() && !scene->contains_entity(name)) {
				if (imgui::MenuItem("New Entity")) {
					kl::Object entity = scene->new_entity(false);
					scene->add_entity(name, entity);
					imgui::CloseCurrentPopup();
				}
				if (imgui::MenuItem("New Camera")) {
					kl::Object entity = scene->new_casted<Camera>(false);
					scene->add_entity(name, entity);
					imgui::CloseCurrentPopup();
				}
				if (imgui::MenuItem("New Ambient Light")) {
					kl::Object entity = scene->new_casted<AmbientLight>(false);
					scene->add_entity(name, entity);
					imgui::CloseCurrentPopup();
				}
				if (imgui::MenuItem("New Point Light")) {
					kl::Object entity = scene->new_casted<PointLight>(false);
					scene->add_entity(name, entity);
					imgui::CloseCurrentPopup();
				}
				if (imgui::MenuItem("New Directional Light")) {
					kl::Object entity = scene->new_casted<DirectionalLight>(false, gpu, 4096);
					scene->add_entity(name, entity);
					imgui::CloseCurrentPopup();
				}
			}
			imgui::EndPopup();
		}

		if (imgui::IsWindowFocused() && imgui::IsMouseHoveringRect(imgui::GetWindowPos(), imgui::GetWindowPos() + imgui::GetWindowSize())) {
			if (imgui::IsKeyPressed(ImGuiKey_Delete)) {
				for (auto& name : editor_layer->selected_entities) {
					scene->remove_entity(name);
				}
				editor_layer->selected_entities.clear();
			}
		}

		// Entities
		const std::string filter = gui_input_continuous("Search###SceneEntities");
		for (auto& [entity_name, entity] : *scene) {
			if (!filter.empty() && entity_name.find(filter) == -1) {
				continue;
			}

			// Entity type
			if (entity.is<Camera>()) {
				imgui::Button("CAMERA");
			}
			else if (entity.is<AmbientLight>()) {
				imgui::Button("AMBIENT");
			}
			else if (entity.is<PointLight>()) {
				imgui::Button("POINT");
			}
			else if (entity.is<DirectionalLight>()) {
				imgui::Button("DIRECTIONAL");
			}
			else {
				imgui::Button("ENTITY");
			}
			imgui::SameLine();

			// Entity name
			if (imgui::Selectable(entity_name.c_str(), editor_layer->selected_entities.contains(entity_name))) {
				if (imgui::IsKeyDown(ImGuiKey_LeftCtrl)) {
					if (editor_layer->selected_entities.contains(entity_name)) {
						editor_layer->selected_entities.erase(entity_name);
					}
					else {
						editor_layer->selected_entities.insert(entity_name);
					}
				}
				else if (imgui::IsKeyDown(ImGuiKey_LeftShift)) {
					if (editor_layer->selected_entities.empty()) {
						editor_layer->selected_entities.insert(entity_name);
					}
					else {
						int find_counter = 0;
						std::vector<std::string> needed_entities;
						for (const auto& [name, _] : *scene) {
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
			if (imgui::BeginPopupContextItem(entity_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
				bool should_break = false;
				imgui::Text("Edit Entity");

				if (std::optional opt_name = gui_input_waited("##RenameEntityInput", entity_name)) {
					const std::string& name = opt_name.value();
					if (!name.empty() && !scene->contains_entity(name)) {
						kl::Object temp_holder = entity;
						scene->remove_entity(entity_name);
						scene->add_entity(name, temp_holder);

						should_break = true;
						imgui::CloseCurrentPopup();
					}
				}

				if (imgui::Button("Delete", { -1.0f, 0.0f })) {
					scene->remove_entity(entity_name);

					should_break = true;
					imgui::CloseCurrentPopup();
				}

				imgui::EndPopup();
				if (should_break) {
					break;
				}
			}
		}
	}
	imgui::End();
}
