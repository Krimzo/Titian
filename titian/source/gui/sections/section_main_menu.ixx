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
        kl::Object<GameLayer> game_layer = nullptr;

        GUISectionMainMenu(kl::Object<RenderLayer>& render_layer, kl::Object<GameLayer>& game_layer)
		{
            this->render_layer = render_layer;
            this->game_layer = game_layer;
        }

		~GUISectionMainMenu() override
		{}

		void render_gui() override
		{
            kl::Object<ApplicationLayer>& app_layer = render_layer->app_layer;

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
                        const bool serial_result = serialize(&game_layer->scene, save_path);

                        if (serial_result) {
                            app_layer->log(kl::format("Scene saved. (", save_path, ") [", SERIAL_VERSION_FORMAT, "]"));
                        }
                        else {
                            app_layer->log(kl::format("Failed to save scene. File write error. (", save_path, ")"));
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
                        game_layer->scene = new Scene(render_layer->app_layer->gpu);
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
