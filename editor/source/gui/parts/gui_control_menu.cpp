#include "editor.h"


bool centered_button(const char* label, float alignment = 0.5f);

void gui_control_menu(editor_state* state)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 10.0f, 5.0f });

	if (ImGui::Begin("Control Menu", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        if (ImGui::BeginTable("ControlTable", 2, ImGuiTableFlags_Borders)) {
            ImGui::TableNextRow();

            // 0
            ImGui::TableSetColumnIndex(0);
            if (state->game_running) {
                if (ImGui::Button("Stop")) {
                    // Change state
                    state->game_running = false;

                    // Load scene
                    deserializer deserializer = { "temp.titian" };
                    std::pair load_info = deserializer.read_scene(state->gpu);
                    if (load_info.first == 0) {
                        state->change_scene(load_info.second);
                        state->logger_state->log(kl::format("Scene loaded. (", deserializer.path, ") [", serialization::VERSION_NAME, "]"));
                    }
                    else if (load_info.first == 1) {
                        state->logger_state->log(kl::format("Failed to load scene. File read error. (", deserializer.path, ")"));
                    }
                    else {
                        state->logger_state->log(kl::format("Failed to load scene. Serial versions do not match. (", deserializer.path, ") [",
                            serialization::VERSION_NAME, " -> ", std::hex, "0x", load_info.first, "]"));
                    }
                }
            }
            else {
                if (ImGui::Button("Start")) {
                    // Save scene
                    serializer serializer = { "temp.titian" };
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
                    state->script_state->call_starts();
                }
            }

            // 1
            ImGui::TableSetColumnIndex(1);
            bool scaling_state = (state->gui_state->gizmo_operation == ImGuizmo::SCALE);
            if (ImGui::Checkbox("Scaling", &scaling_state)) {
                state->gui_state->gizmo_operation = scaling_state ? ImGuizmo::SCALE : NULL;
            }
            ImGui::SameLine();

            bool rotation_state = (state->gui_state->gizmo_operation == ImGuizmo::ROTATE);
            if (ImGui::Checkbox("Rotation", &rotation_state)) {
                state->gui_state->gizmo_operation = rotation_state ? ImGuizmo::ROTATE : NULL;
            }
            ImGui::SameLine();

            bool translation_state = (state->gui_state->gizmo_operation == ImGuizmo::TRANSLATE);
            if (ImGui::Checkbox("Translation", &translation_state)) {
                state->gui_state->gizmo_operation = translation_state ? ImGuizmo::TRANSLATE : NULL;
            }
            ImGui::SameLine();

            ImGui::EndTable();
        }
	}
	ImGui::End();

    ImGui::PopStyleVar(2);
}

bool centered_button(const char* label, const float alignment)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}
