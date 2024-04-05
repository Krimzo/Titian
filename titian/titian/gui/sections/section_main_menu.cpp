#include "main.h"


titian::GUISectionMainMenu::GUISectionMainMenu(EditorLayer* editor_layer, GUILayer* gui_layer)
    : GUISection("GUISectionMainMenu")
    , editor_layer(editor_layer)
    , gui_layer(gui_layer)
{
    kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;

    m_start_button_texture = new Texture(gpu);
    m_pause_button_texture = new Texture(gpu);
    m_stop_button_texture = new Texture(gpu);

    m_start_button_texture->data_buffer.load_from_file("builtin/textures/start_button.png");
    m_pause_button_texture->data_buffer.load_from_file("builtin/textures/pause_button.png");
    m_stop_button_texture->data_buffer.load_from_file("builtin/textures/stop_button.png");

    m_start_button_texture->load_as_2D();
    m_pause_button_texture->load_as_2D();
    m_stop_button_texture->load_as_2D();

    m_start_button_texture->create_shader_view();
    m_pause_button_texture->create_shader_view();
    m_stop_button_texture->create_shader_view();

    kl::assert(m_start_button_texture->shader_view, "Failed to init start_button texture.");
    kl::assert(m_pause_button_texture->shader_view, "Failed to init pause_button texture.");
    kl::assert(m_stop_button_texture->shader_view, "Failed to init stop_button texture.");
}

void titian::GUISectionMainMenu::render_gui()
{
    const TimeBomb _ = this->time_it();

    RenderLayer* render_layer = gui_layer->render_layer;
    GameLayer* game_layer = editor_layer->game_layer;
    AppLayer* app_layer = game_layer->app_layer;

    kl::Object<Scene>& scene = game_layer->scene;
    kl::Window* window = &app_layer->window;

    // Save scene popup
    if (m_inputting_name) {
        if (ImGui::Begin("Save Scene", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetNextItemWidth(-1.0f);
            if (std::optional opt_name = gui_input_waited("##SaveSceneNameInput", {})) {
                const std::string& name = opt_name.value();
                if (!name.empty()) {
                    const std::string save_path = kl::format(opt_name.value(), FILE_EXTENSION_SCENE);
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
    if (m_testing_exit) {
        if (ImGui::Begin("Exit?", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::Text("Are you sure you want to exit?");
            if (ImGui::Button("No", ImVec2(45.0f, 20.0f))) {
                m_testing_exit = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Yes", ImVec2(35.0f, 20.0f))) {
                app_layer->window->close();
            }
        }
        ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4) ImColor(0, 0, 0));

    if (ImGui::BeginMainMenuBar()) {
        const ImVec2 menu_size = ImGui::GetContentRegionAvail();

        // File
        if (ImGui::BeginMenu("File")) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (ImGui::MenuItem("New Scene")) {
                scene = new Scene(&app_layer->gpu);
            }
            if (ImGui::MenuItem("Save Scene")) {
                m_inputting_name = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                m_testing_exit = true;
            }
            ImGui::PopStyleVar();
            ImGui::EndMenu();
        }

        // Edit
        if (ImGui::BeginMenu("Edit")) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (ImGui::MenuItem("Undo")) {
            }
            if (ImGui::MenuItem("Redo")) {
            }
            ImGui::PopStyleVar();
            ImGui::EndMenu();
        }

        // View
        if (ImGui::BeginMenu("View")) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (ImGui::BeginMenu("Features")) {
                ImGui::Checkbox("Render Wireframe", &render_layer->render_wireframe);
                ImGui::Checkbox("Use vSync", &render_layer->v_sync);
                ImGui::EndMenu();
            }
            ImGui::PopStyleVar();
            ImGui::EndMenu();
        }

        // About
        if (ImGui::BeginMenu("About")) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            ImGui::Text("Titian 0.64v");
            ImGui::PopStyleVar();
            ImGui::EndMenu();
        }

        // Game control
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(30, 30, 30));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        ImGui::SetCursorPosX((menu_size.x - m_control_buttons_width) * 0.5f);
        m_control_buttons_width = 0.0f;

        const float button_image_size = 16.0f;
        
        const bool is_start_enabled = !game_layer->game_running || game_layer->game_paused;
        const ImVec4 start_tint_color = is_start_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !is_start_enabled);
        if (ImGui::ImageButton(m_start_button_texture->shader_view.Get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), start_tint_color)) {
            if (game_layer->game_paused) {
                game_layer->resume_game();
            }
            else {
                if (Serializer serializer = { m_temp_path, true }) {
                    game_layer->scene->serialize(&serializer, nullptr);
                }
                game_layer->start_game();
            }
        }
        m_control_buttons_width += ImGui::GetItemRectSize().x;
        ImGui::PopItemFlag();

        const bool is_pause_enabled = game_layer->game_running && !game_layer->game_paused;
        const ImVec4 pause_tint_color = is_pause_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !is_pause_enabled);
        if (ImGui::ImageButton(m_pause_button_texture->shader_view.Get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), pause_tint_color)) {
            game_layer->pause_game();
        }
        m_control_buttons_width += ImGui::GetItemRectSize().x;
        ImGui::PopItemFlag();

        const bool is_stop_enabled = game_layer->game_running || game_layer->game_paused;
        const ImVec4 stop_tint_color = is_stop_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !is_stop_enabled);
        if (ImGui::ImageButton(m_stop_button_texture->shader_view.Get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), stop_tint_color)) {
            game_layer->stop_game();
            game_layer->reset_scene();
            if (const Serializer serializer = { m_temp_path, false }) {
                game_layer->scene->deserialize(&serializer, nullptr);
            }
        }
        m_control_buttons_width += ImGui::GetItemRectSize().x;
        ImGui::PopItemFlag();

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);

#if 0
        // Gizmo control
        ImGui::SetCursorPosX(menu_size.x - m_gizmo_buttons_width);
        m_gizmo_buttons_width = 0.0f;

        if (ImGui::BeginMenu("Gizmos")) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

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

            ImGui::PopStyleVar();
            ImGui::EndMenu();
        }
        m_gizmo_buttons_width += ImGui::GetItemRectSize().x;
#endif

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}
