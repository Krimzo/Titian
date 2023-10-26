export module section_main_menu;

export import gui_section;
export import render_layer;
export import editor_layer;
export import serialization;

export namespace titian {
	class GUISectionMainMenu : public GUISection
	{
    public:
        kl::Object<RenderLayer> render_layer = nullptr;

        GUISectionMainMenu(kl::Object<RenderLayer>& render_layer)
		{
            this->render_layer = render_layer;
        }

		~GUISectionMainMenu() override
		{}

		void render_gui() override
		{
            AppLayer* app_layer = &render_layer->game_layer->app_layer;
            kl::Object<Scene>& scene = render_layer->game_layer->scene;

            if (m_inputting_name) {
                if (ImGui::Begin("Save Scene", nullptr, ImGuiWindowFlags_NoScrollbar)) {
                    static char name_input[31] = {};
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::InputText("##SaveSceneNameInput", name_input, std::size(name_input));

                    if (ImGui::Button("Cancel")) {
                        memset(name_input, 0, std::size(name_input));
                        m_inputting_name = false;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Save")) {
                        const std::string save_path = kl::format(name_input, ".titian");
                        const bool serial_result = serialize(save_path, &scene, nullptr);

                        if (serial_result) {
                            Logger::log(kl::format("Scene saved. (", save_path, ") [", SERIAL_VERSION_FORMAT, "]"));
                        }
                        else {
                            Logger::log(kl::format("Failed to save scene. File write error. (", save_path, ")"));
                        }
                        memset(name_input, 0, std::size(name_input));
                        m_inputting_name = false;
                    }
                }
                ImGui::End();
            }

            if (ImGui::BeginMainMenuBar()) {
                ImGui::Text("TITIAN");

                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New Scene")) {
                        scene = new Scene(app_layer->gpu);
                    }

                    if (ImGui::MenuItem("Save Scene")) {
                        m_inputting_name = true;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit")) {
                    if (ImGui::MenuItem("Undo")) {

                    }

                    if (ImGui::MenuItem("Redo")) {

                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View")) {
                    if (ImGui::BeginMenu("Features")) {
                        ImGui::Checkbox("Use vSync", &render_layer->v_sync);
                        ImGui::Checkbox("Render Wireframe", &render_layer->render_wireframe);
                        ImGui::Checkbox("Render Colliders", &render_layer->render_colliders);

                        ImGui::EndMenu();
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("About")) {
                    ImGui::Text("Titian 0.4v");

                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
		}

    private:
        bool m_inputting_name = false;
	};
}
