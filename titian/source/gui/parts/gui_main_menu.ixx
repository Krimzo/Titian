export module gui_main_menu;

export import gui_render;
export import serializer;
export import editor_setup;

export void gui_main_menu(EditorState* state)
{
    static bool inputting_name = false;

    if (inputting_name) {
        if (ImGui::Begin("Save Scene", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            static char name_input[31] = {};
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputText("##SaveSceneNameInput", name_input, std::size(name_input));

            if (ImGui::Button("Cancel")) {
                memset(name_input, 0, std::size(name_input));
                inputting_name = false;
            }

            ImGui::SameLine();
            if (ImGui::Button("Save")) {
                Serializer serializer = { kl::format(name_input, ".titian") };
                if (serializer.write_scene(state->scene)) {
                    state->logger_state->log(kl::format("Scene saved. (", serializer.path, ") [", serialization::VERSION_NAME, "]"));
                }
                else {
                    state->logger_state->log(kl::format("Failed to save scene. File write error. (", serializer.path, ")"));
                }
                memset(name_input, 0, std::size(name_input));
                inputting_name = false;
            }
        }
        ImGui::End();
    }

    if (ImGui::BeginMainMenuBar()) {
        ImGui::Text("TITIAN");

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {
                state->change_scene(new kl::Scene());
            }

            if (ImGui::MenuItem("Save Scene")) {
                inputting_name = true;
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
                ImGui::Checkbox("Use vSync", &state->v_sync);
                ImGui::Checkbox("Render Wireframe", &state->render_wireframe);
                ImGui::Checkbox("Render Colliders", &state->gui_state->render_collider);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Colors")) {
                ImGui::ColorEdit3("Special", state->gui_state->color_special);
                ImGui::ColorEdit3("Alternative", state->gui_state->color_alternative);

                if (ImGui::Button("Reload")) {
                    load_custom_theme(state);
                }

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