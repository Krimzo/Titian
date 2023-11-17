#include "main.h"


titian::GUISectionSceneEntities::GUISectionSceneEntities(EditorLayer* editor_layer)
{
	this->editor_layer = editor_layer;
}

void titian::GUISectionSceneEntities::render_gui()
{
	kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
	Scene* scene = &editor_layer->game_layer->scene;

	if (ImGui::Begin("Scene Entities")) {
		// Window popup
		if (ImGui::BeginPopupContextWindow("NewEntity", ImGuiPopupFlags_MouseButtonMiddle)) {
			const std::string name = gui_input_continuous("##CreateEntityInput");
			if (!name.empty()) {
				if (ImGui::MenuItem("New Entity") && !scene->contains_entity(name)) {
					kl::Object entity = scene->new_entity(false);
					scene->add_entity(name, entity);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("New Camera") && !scene->contains_entity(name)) {
					kl::Object entity = scene->new_casted<Camera>(false);
					scene->add_entity(name, entity);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("New Ambient Light") && !scene->contains_entity(name)) {
					kl::Object entity = scene->new_casted<AmbientLight>(false);
					scene->add_entity(name, entity);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("New Point Light") && !scene->contains_entity(name)) {
					kl::Object entity = scene->new_casted<PointLight>(false);
					scene->add_entity(name, entity);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("New Directional Light") && !scene->contains_entity(name)) {
					kl::Object entity = scene->new_casted<DirectionalLight>(false, gpu, 4096);
					scene->add_entity(name, entity);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		// Entities
		const std::string filter = gui_input_continuous("Search###SceneEntities");
		for (auto& [entity_name, entity] : *scene) {
			if (!filter.empty() && entity_name.find(filter) == -1) {
				continue;
			}

			// Entity type
			if (dynamic_cast<Camera*>(&entity)) {
				ImGui::Button("CAMERA");
			}
			else if (dynamic_cast<AmbientLight*>(&entity)) {
				ImGui::Button("AMBIENT");
			}
			else if (dynamic_cast<PointLight*>(&entity)) {
				ImGui::Button("POINT");
			}
			else if (dynamic_cast<DirectionalLight*>(&entity)) {
				ImGui::Button("DIRECTIONAL");
			}
			else {
				ImGui::Button("ENTITY");
			}
			ImGui::SameLine();

			// Entity name
			if (ImGui::Selectable(entity_name.c_str(), entity_name == editor_layer->selected_entity)) {
				editor_layer->selected_entity = entity_name;
			}

			// Item popup
			if (ImGui::BeginPopupContextItem(entity_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
				bool should_break = false;
				ImGui::Text("Edit Entity");

				if (std::optional opt_name = gui_input_waited("##RenameEntityInput", entity_name)) {
					const std::string& name = opt_name.value();
					if (!name.empty() && !scene->contains_entity(name)) {
						kl::Object temp_holder = entity;
						scene->remove_entity(entity_name);
						scene->add_entity(name, temp_holder);

						should_break = true;
						ImGui::CloseCurrentPopup();
					}
				}

				if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
					scene->remove_entity(entity_name);

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