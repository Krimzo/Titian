export module section_control_menu;

export import gui_section;
export import editor_layer;

export namespace titian {
    class GUISectionControlMenu : public GUISection
    {
    public:
        EditorLayer* editor_layer = nullptr;
        
        GUISectionControlMenu(EditorLayer* editor_layer)
        {
            this->editor_layer = editor_layer;
        }

        ~GUISectionControlMenu() override
        {}

        void render_gui() override
        {
            if (ImGui::Begin("Control Menu", nullptr, ImGuiWindowFlags_NoScrollbar)) {
                // Game
                ImGui::Text("Game");
                
                if (!editor_layer->game_layer->game_running) {
                    if (ImGui::Button("Start Game", { -1.0f, 0.0f })) {
                        start_scene();
                    }
                }
                else {
                    if (ImGui::Button("Stop Game", { -1.0f, 0.0f })) {
                        stop_scene();
                    }
                }

                // Gizmos
                ImGui::Separator();
                ImGui::Text("Gizmos");

                bool scaling_state = editor_layer->gizmo_operation == ImGuizmo::SCALE;
                if (ImGui::Checkbox("Scaling", &scaling_state)) {
                    editor_layer->gizmo_operation = scaling_state ? ImGuizmo::SCALE : 0;
                }

                bool rotation_state = editor_layer->gizmo_operation == ImGuizmo::ROTATE;
                if (ImGui::Checkbox("Rotation", &rotation_state)) {
                    editor_layer->gizmo_operation = rotation_state ? ImGuizmo::ROTATE : 0;
                }

                bool translation_state = editor_layer->gizmo_operation == ImGuizmo::TRANSLATE;
                if (ImGui::Checkbox("Translation", &translation_state)) {
                    editor_layer->gizmo_operation = translation_state ? ImGuizmo::TRANSLATE : 0;
                }
            }
            ImGui::End();
        }

    private:
        const std::string m_temp_path = "temp.titian";

        void start_scene()
        {
            GameLayer* game_layer = editor_layer->game_layer;

            // Cache scene
            if (Serializer serializer = { m_temp_path, true }) {
                game_layer->scene->serialize(&serializer, nullptr);
            }

            // Start game
            game_layer->start_game();
        }

        void stop_scene()
        {
            GameLayer* game_layer = editor_layer->game_layer;

            // Stop game
            game_layer->stop_game();
            game_layer->reset_scene();

            // Load cached scene
            if (const Serializer serializer = { m_temp_path, false }) {
                game_layer->scene->deserialize(&serializer, nullptr);
            }
        }
    };
}
