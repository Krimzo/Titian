#include "fuze.h"


fuze::GUISectionVideoMainMenu::GUISectionVideoMainMenu(const LayerPackage& package)
	: GUISection("GUISectionVideoMainMenu", package)
{}

void fuze::GUISectionVideoMainMenu::render_gui()
{
    if (m_testing_exit) {
        if (im::Begin("Exit?", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            im::Text("Are you sure you want to exit?");
            if (im::Button("No", ImVec2(50.0f, 0.0f))) {
                m_testing_exit = false;
            }
            im::SameLine();
            if (im::Button("Yes", ImVec2(40.0f, 0.0f))) {
                app_layer->window.close();
            }
        }
        im::End();
    }

    im::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
    im::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4) ImColor(0, 0, 0));

    if (im::BeginMainMenuBar()) {
        // File
        if (im::BeginMenu("File")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::BeginMenu("Import")) {
                im::EndMenu();
            }
            if (im::BeginMenu("Export")) {
                im::EndMenu();
            }
            im::Separator();
            if (im::MenuItem("Exit")) {
                m_testing_exit = true;
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        // Edit
        if (im::BeginMenu("Edit")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::MenuItem("Undo")) {
            }
            if (im::MenuItem("Redo")) {
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        // Tools
        if (im::BeginMenu("Tools")) {
            im::EndMenu();
        }

        // View
        if (im::BeginMenu("Render")) {
            im::EndMenu();
        }

        // About
        if (im::BeginMenu("About")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            im::Text("Fuze 0.01v");
            im::PopStyleVar();
            im::EndMenu();
        }

        im::EndMainMenuBar();
    }

    im::PopStyleColor();
    im::PopStyleVar(2);
}
