export module section_control_menu;

export import gui_section;
export import editor_layer;

export namespace titian {
    class GUISectionControlMenu : public GUISection
    {
    public:
        kl::Object<EditorLayer> editor_layer = nullptr;
        
        GUISectionControlMenu(kl::Object<EditorLayer>& editor_layer)
        {
            this->editor_layer = editor_layer;
        }

        ~GUISectionControlMenu() override
        {}

        void render_gui() override
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 10.0f, 5.0f });

            if (ImGui::Begin("Control Menu", nullptr, ImGuiWindowFlags_NoScrollbar)) {
                if (ImGui::BeginTable("ControlTable", 2, ImGuiTableFlags_Borders)) {
                    ImGui::TableNextRow();

                    // 0
                    ImGui::TableSetColumnIndex(0);
                    if (!editor_layer->game_layer->game_running) {
                        if (ImGui::Button("Start Game")) {
                            start_scene();
                        }
                    }
                    else {
                        if (ImGui::Button("Stop Game")) {
                            stop_scene();
                        }
                    }

                    // 1
                    ImGui::TableSetColumnIndex(1);
                    bool scaling_state = editor_layer->gizmo_operation == ImGuizmo::SCALE;
                    if (ImGui::Checkbox("Scaling", &scaling_state)) {
                        editor_layer->gizmo_operation = scaling_state ? ImGuizmo::SCALE : 0;
                    }
                    ImGui::SameLine();

                    bool rotation_state = editor_layer->gizmo_operation == ImGuizmo::ROTATE;
                    if (ImGui::Checkbox("Rotation", &rotation_state)) {
                        editor_layer->gizmo_operation = rotation_state ? ImGuizmo::ROTATE : 0;
                    }
                    ImGui::SameLine();

                    bool translation_state = editor_layer->gizmo_operation == ImGuizmo::TRANSLATE;
                    if (ImGui::Checkbox("Translation", &translation_state)) {
                        editor_layer->gizmo_operation = translation_state ? ImGuizmo::TRANSLATE : 0;
                    }
                    ImGui::SameLine();

                    ImGui::EndTable();
                }
            }
            ImGui::End();

            ImGui::PopStyleVar(2);
        }

    private:
        std::string m_temp_path = "temp.titian";

        void start_scene()
        {
            GameLayer* game_layer = &editor_layer->game_layer;

            // Reload all scripts
            for (auto& [_, script] : game_layer->scene->scripts) {
                script->reload();
            }

            // Save scene
            Serializer serializer = { m_temp_path, true };
            game_layer->scene->serialize(&serializer);

            // Change state
            game_layer->game_running = true;
            game_layer->app_layer->timer->reset_elapsed();

            // Call script starts
            for (auto& [_, script] : game_layer->scene->scripts) {
                script->call_start();
            }
        }

        void stop_scene()
        {
            GameLayer* game_layer = &editor_layer->game_layer;

            // Change state
            game_layer->game_running = false;
            game_layer->scene = new Scene(game_layer->app_layer->gpu);

            // Load scene
            const Serializer serializer = { m_temp_path, false };
            game_layer->scene->deserialize(&serializer);
        }
    };
}
