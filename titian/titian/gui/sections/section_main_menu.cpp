#include "main.h"


titian::GUISectionMainMenu::GUISectionMainMenu(RenderLayer* render_layer)
{
    this->render_layer = render_layer;
}

void titian::GUISectionMainMenu::render_gui()
{
    AppLayer* app_layer = render_layer->game_layer->app_layer;
    kl::Object<Scene>& scene = render_layer->game_layer->scene;

    if (m_inputting_name) {
        if (ImGui::Begin("Save Scene", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetNextItemWidth(-1.0f);
            if (std::optional opt_name = gui_input_waited("##SaveSceneNameInput", {})) {
                const std::string& name = opt_name.value();
                if (!name.empty()) {
                    const std::string save_path = kl::format(opt_name.value(), ".titian");
                    if (Serializer serializer = { save_path, true }) {
                        scene->serialize(&serializer, nullptr);
                    }
                    m_inputting_name = false;
                }
            }
            if (ImGui::Button("Cancel")) {
                m_inputting_name = false;
            }
        }
        ImGui::End();
    }

    if (ImGui::BeginMainMenuBar()) {
        ImGui::Text("TITIAN");

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {
                scene = new Scene(&app_layer->gpu);
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
                ImGui::Checkbox("Render Wireframe", &render_layer->render_wireframe);
                ImGui::Checkbox("Use vSync", &render_layer->v_sync);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("About")) {
            ImGui::Text("Titian 0.6v");

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
