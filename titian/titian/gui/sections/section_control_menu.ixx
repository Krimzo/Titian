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
                    if (!editor_layer->game_running) {
                        if (ImGui::Button("Start")) {
                            start_scene();
                        }
                    }
                    else {
                        if (ImGui::Button("Stop")) {
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
        void start_scene()
        {
            // Save scene
            Serializer serializer = { "temp.titian", true };
            editor_layer->game_layer->scene->serialize(&serializer);

            // Change state
            editor_layer->game_running = true;
            editor_layer->game_layer->app_layer->timer->reset_elapsed();

            // Call script starts
            //state->scripts_call_start(state);
        }

        void stop_scene()
        {
            // Change state
            editor_layer->game_running = false;

            // Load scene
            const Serializer serializer = { "temp.titian", false };
            editor_layer->game_layer->scene = new Scene(editor_layer->game_layer->app_layer->gpu);
            editor_layer->game_layer->scene->deserialize(&serializer);
        }
    };
}
