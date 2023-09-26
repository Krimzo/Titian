export module gui_control_menu;

export import gui_render;
export import serializer;
export import deserializer;

void start_scene(EditorState* state);
void stop_scene(EditorState* state);

export void gui_control_menu(EditorState* state)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 10.0f, 5.0f });

	if (ImGui::Begin("Control Menu", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        if (ImGui::BeginTable("ControlTable", 2, ImGuiTableFlags_Borders)) {
            ImGui::TableNextRow();

            // 0
            ImGui::TableSetColumnIndex(0);
            if (!state->game_running) {
                if (ImGui::Button("Start")) {
                    start_scene(state);
                }
            }
            else {
                if (ImGui::Button("Stop")) {
                    stop_scene(state);
                }
            }

            // 1
            ImGui::TableSetColumnIndex(1);
            bool scaling_state = (state->gui_state->gizmo_operation == ImGuizmo::SCALE);
            if (ImGui::Checkbox("Scaling", &scaling_state)) {
                state->gui_state->gizmo_operation = scaling_state ? ImGuizmo::SCALE : 0;
            }
            ImGui::SameLine();

            bool rotation_state = (state->gui_state->gizmo_operation == ImGuizmo::ROTATE);
            if (ImGui::Checkbox("Rotation", &rotation_state)) {
                state->gui_state->gizmo_operation = rotation_state ? ImGuizmo::ROTATE : 0;
            }
            ImGui::SameLine();

            bool translation_state = (state->gui_state->gizmo_operation == ImGuizmo::TRANSLATE);
            if (ImGui::Checkbox("Translation", &translation_state)) {
                state->gui_state->gizmo_operation = translation_state ? ImGuizmo::TRANSLATE : 0;
            }
            ImGui::SameLine();

            ImGui::EndTable();
        }
	}
	ImGui::End();

    ImGui::PopStyleVar(2);
}

void start_scene(EditorState* state)
{
    // Save scene
    Serializer serializer = { "temp.titian" };
    if (serializer.write_scene(state->scene)) {
        state->logger_state->log(kl::format("Scene saved. (", serializer.path, ") [", serialization::VERSION_NAME, "]"));
    }
    else {
        state->logger_state->log(kl::format("Failed to save scene. File write error. (", serializer.path, ")"));
    }

    // Change state
    state->game_running = true;

    // Reset times
    state->timer.reset_elapsed();
    state->gui_state->render_info.last_update_time = 0.0f;

    // Call script starts
    state->scripts_call_start(state);
}

void stop_scene(EditorState* state)
{
    // Change state
    state->game_running = false;

    // Load scene
    Deserializer deserializer = { "temp.titian" };
    const auto [error, scene] = deserializer.read_scene(state->gpu);
    if (error == 0) {
        state->change_scene(scene);
        state->logger_state->log(kl::format("Scene loaded. (", deserializer.path, ") [", serialization::VERSION_NAME, "]"));
    }
    else if (error == 1) {
        state->logger_state->log(kl::format("Failed to load scene. File read error. (", deserializer.path, ")"));
    }
    else {
        state->logger_state->log(kl::format("Failed to load scene. Serial versions do not match. (", deserializer.path, ") [",
            serialization::VERSION_NAME, " -> ", std::hex, "0x", error, "]"));
    }
}
