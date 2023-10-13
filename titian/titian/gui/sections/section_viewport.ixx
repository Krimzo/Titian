export module section_viewport;

export import gui_section;
export import render_layer;

export namespace titian {
	class GUISectionViewport : public GUISection
	{
	public:
        kl::Object<RenderLayer> render_layer = nullptr;

		GUISectionViewport(kl::Object<RenderLayer>& render_layer)
		{
            this->render_layer = render_layer;
        }

		~GUISectionViewport() override
		{}

		void render_gui() override
		{
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

            if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
                // Pre-display
                const ImVec2 content_region = ImGui::GetContentRegionAvail();
                render_layer->resize({ (int) content_region.x, (int) content_region.y });
                //state->gui_state->is_viewport_focused = ImGui::IsWindowFocused();

                // Display rendered texture
                ImGui::Image(render_layer->render_texture->shader_view.Get(), content_region);
                
                /*
                // Scene loading
                const std::optional scene_file = gui::drag_drop::read_data<std::string>("SceneFile");
                if (scene_file) {
                    const Deserializer deserializer = { scene_file.value() };
                    const auto [error, scene] = deserializer.read_scene(state->gpu);
                    if (error == 0) {
                        state->change_scene(scene);
                        state->logger_state->log(kl::format("Scene loaded. (", deserializer.path, ") [", serialization::VERSION_NAME, "]"));
                    }
                    else if (error == 1) {
                        state->logger_state->log(kl::format("Failed to load scene. File read error. (", deserializer.path, ")"));
                    }
                    else {
                        state->logger_state->log(kl::format("Failed to load scene. Serial versions do not match. (", deserializer.path, ") [", serialization::VERSION_NAME, " -> ", std::hex, "0x", error, "]"));
                    }
                }

                // Handle entity picking
                const ImVec2 viewport_max = { ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight() };
                if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), viewport_max) && !ImGuizmo::IsOver()) {
                    const kl::Int2 pixel_coords = window_mouse_position();
                    const int entity_index = find_entity_index(state, pixel_coords);
                    state->scene->update_selected_entity(entity_index);
                }

                // Handle gizmos
                if (ImGui::IsWindowFocused()) {
                    handle_gizmo_operation_change(state, ImGuizmo::OPERATION::SCALE, ImGuiKey_1);
                    handle_gizmo_operation_change(state, ImGuizmo::OPERATION::ROTATE, ImGuiKey_2);
                    handle_gizmo_operation_change(state, ImGuizmo::OPERATION::TRANSLATE, ImGuiKey_3);
                }
                if (state->scene->selected_entity) {
                    render_gizmos(state);
                }
                */
            }
            ImGui::End();

            ImGui::PopStyleVar();
		}

    private:

	};
}
