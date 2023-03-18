#include "gui/gui_render.h"


void gui_main_menu(editor_state* state)
{
    if (ImGui::BeginMainMenuBar()) {
        ImGui::Text("TITIAN");

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Project")) {
                
            }

            if (ImGui::MenuItem("Save Project")) {

            }

            if (ImGui::MenuItem("Load Project")) {

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
                ImGui::Checkbox("Render Colliders", &state->gui_state.render_collider);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Colors")) {
                ImGui::ColorEdit3("Selection Color", state->gui_state.selection_color);
                ImGui::ColorEdit3("Collider Color", state->gui_state.collider_color);
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("About")) {
            ImGui::Text("Titian 0.2v");

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
