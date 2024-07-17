#include "main.h"


titian::GUISectionMainMenu::GUISectionMainMenu(const LayerPackage& package)
    : GUISection("GUISectionMainMenu", package)
{
    auto create_texture = [&](kl::Object<Texture>& texture, const char* filename)
    {
        texture = new Texture(&app_layer->gpu);
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D(false, false);
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(m_start_button_texture, "builtin/textures/start_button.png"); });
    queue.add_task([&] { create_texture(m_pause_button_texture, "builtin/textures/pause_button.png"); });
    queue.add_task([&] { create_texture(m_stop_button_texture, "builtin/textures/stop_button.png"); });
    queue.finalize();
}

void titian::GUISectionMainMenu::render_gui()
{
    const TimeBomb _ = this->time_it();

    kl::Object<Scene>& scene = game_layer->scene;
    kl::Window* window = &app_layer->window;

    // Save scene popup
    if (m_inputting_name) {
        if (imgui::Begin("Save Scene", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            imgui::SetNextItemWidth(-1.0f);
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
            if (imgui::Button("Cancel")) {
                m_inputting_name = false;
            }
        }
        imgui::End();
    }
    if (m_testing_exit) {
        if (imgui::Begin("Exit?", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            imgui::Text("Are you sure you want to exit?");
            if (imgui::Button("No", ImVec2(50.0f, 0.0f))) {
                m_testing_exit = false;
            }
            imgui::SameLine();
            if (imgui::Button("Yes", ImVec2(40.0f, 0.0f))) {
                app_layer->window.close();
            }
        }
        imgui::End();
    }

    imgui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
    imgui::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4) ImColor(0, 0, 0));

    if (imgui::BeginMainMenuBar()) {
        const ImVec2 menu_size = imgui::GetContentRegionAvail();

        // File
        if (imgui::BeginMenu("File")) {
            imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (imgui::BeginMenu("Import")) {
                if (imgui::MenuItem("Mesh")) {
                    if (std::optional file = kl::choose_file(false, { { "Mesh File",  FILE_EXTENSION_MESH } })) {
                        const std::string stem_name = std::filesystem::path(file.value()).stem().string();

                        Mesh* mesh = scene->helper_new_mesh(stem_name);
                        mesh->data_buffer = kl::parse_obj_file(file.value());
                        mesh->reload();
                    }
                }
                if (imgui::MenuItem("Texture")) {
                    if (std::optional file = kl::choose_file(false, { { "Texture File", FILE_EXTENSION_JPG }, { "Texture File", FILE_EXTENSION_PNG } })) {
                        const std::string stem_name = std::filesystem::path(file.value()).stem().string();

                        Texture* texture = scene->helper_new_texture(stem_name);
                        texture->data_buffer.load_from_file(file.value());
                        texture->reload_as_2D();
                        texture->create_shader_view();
                    }
                }
                if (imgui::MenuItem("Script")) {
                    int type_index = 0;
                    if (std::optional file = kl::choose_file(false, { { "Script File", FILE_EXTENSION_INTERP_SCRIPT }, { "Script File", FILE_EXTENSION_NATIVE_SCRIPT } }, &type_index)) {
                        const std::string stem_name = std::filesystem::path(file.value()).stem().string();

                        if (type_index == 0) {
                            kl::Object<NativeScript> script = new NativeScript();
                            script->data = kl::read_file(file.value());
							script->reload();
                            scene->scripts[stem_name] = script;
                        }
                        else if (type_index == 1) {
                            kl::Object<InterpScript> script = new InterpScript();
                            script->source = kl::read_file_string(file.value());
                            script->reload();
                            scene->scripts[stem_name] = script;
                        }
                    }
                }
                if (imgui::MenuItem("Shader")) {
                    if (std::optional file = kl::choose_file(false, { { "Shader File", FILE_EXTENSION_SHADER } })) {
                        const std::string stem_name = std::filesystem::path(file.value()).stem().string();

                        Shader* shader = scene->helper_new_shader(stem_name);
                        shader->data_buffer = kl::read_file_string(file.value());
                    }
                }
                if (imgui::MenuItem("Scene")) {
                    if (std::optional file = kl::choose_file(false, { { "Scene File", FILE_EXTENSION_SCENE } })) {
                        const std::string stem_name = std::filesystem::path(file.value()).stem().string();

                        Serializer serializer{ file.value(), false };
                        if (serializer) {
                            kl::Object scene = new Scene(&app_layer->gpu);
                            scene->deserialize(&serializer, nullptr);
                            game_layer->scene = scene;
                        }
                    }
                }
                imgui::EndMenu();
            }
            if (imgui::BeginMenu("Export")) {
                if (imgui::BeginMenu("Texture")) {
                    const std::string filter = gui_input_continuous("Search###MenuExportTexture");
                    for (auto& [name, texture] : scene->textures) {
                        if (!filter.empty() && name.find(filter) == -1) {
                            continue;
                        }
                        if (imgui::MenuItem(name.c_str())) {
                            int type_index = 0;
                            if (std::optional file = kl::choose_file(true, { { "Texture File",  FILE_EXTENSION_JPG }, { "Texture File",  FILE_EXTENSION_PNG } }, &type_index)) {
                                const std::string extension = std::filesystem::path(file.value()).extension().string();
                                if (type_index == 0) {
                                    if (extension.empty()) {
										file.value() += FILE_EXTENSION_JPG;
                                    }
                                    texture->data_buffer.save_to_file(file.value(), kl::ImageType::JPG);
                                }
                                else if (type_index == 1) {
                                    if (extension.empty()) {
                                        file.value() += FILE_EXTENSION_PNG;
                                    }
                                    texture->data_buffer.save_to_file(file.value(), kl::ImageType::PNG);
                                }
                            }
                        }
                    }
                    imgui::EndMenu();
                }
                if (imgui::BeginMenu("Script")) {
                    const std::string filter = gui_input_continuous("Search###MenuExportScript");
                    for (auto& [name, script] : scene->scripts) {
                        if (!filter.empty() && name.find(filter) == -1) {
                            continue;
                        }
                        if (imgui::MenuItem(name.c_str())) {
                            const std::pair<std::string, std::string> type_info = script.is<InterpScript>() ? std::pair{ "Interp Script", FILE_EXTENSION_INTERP_SCRIPT } : std::pair{ "Native Script", FILE_EXTENSION_NATIVE_SCRIPT };
                            if (std::optional file = kl::choose_file(true, { type_info })) {
                                const std::string extension = std::filesystem::path(file.value()).extension().string();
                                if (InterpScript* interp_script = &script.as<InterpScript>()) {
                                    if (extension.empty()) {
                                        file.value() += FILE_EXTENSION_INTERP_SCRIPT;
                                    }
                                    kl::write_file_string(file.value(), interp_script->source);
                                }
                                else if (NativeScript* native_script = &script.as<NativeScript>()) {
                                    if (extension.empty()) {
                                        file.value() += FILE_EXTENSION_NATIVE_SCRIPT;
                                    }
									kl::write_file(file.value(), native_script->data);
                                }
                            }
                        }
                    }
                    imgui::EndMenu();
                }
                if (imgui::BeginMenu("Shader")) {
                    const std::string filter = gui_input_continuous("Search###MenuExportShader");
                    for (auto& [name, shader] : scene->shaders) {
                        if (!filter.empty() && name.find(filter) == -1) {
                            continue;
                        }
                        if (imgui::MenuItem(name.c_str())) {
                            if (std::optional file = kl::choose_file(true, { { "Shader File", FILE_EXTENSION_SHADER } })) {
                                const std::string extension = std::filesystem::path(file.value()).extension().string();
                                if (extension.empty()) {
                                    file.value() += FILE_EXTENSION_SHADER;
                                }
                                kl::write_file_string(file.value(), shader->data_buffer);
                            }
                        }
                    }
                    imgui::EndMenu();
                }
                if (imgui::MenuItem("Scene")) {
                    if (std::optional file = kl::choose_file(true, { { "Scene File", FILE_EXTENSION_SCENE } })) {
                        const std::string extension = std::filesystem::path(file.value()).extension().string();
                        if (extension.empty()) {
                            file.value() += FILE_EXTENSION_SCENE;
                        }
                        Serializer serializer{ file.value(), true };
                        if (serializer) {
                            scene->serialize(&serializer, nullptr);
                        }
                    }
                }
                imgui::EndMenu();
            }
            imgui::Separator();
            if (imgui::MenuItem("New Scene")) {
                scene = new Scene(&app_layer->gpu);
            }
            if (imgui::MenuItem("Save Scene")) {
                m_inputting_name = true;
            }
            imgui::Separator();
            if (imgui::MenuItem("Exit")) {
                m_testing_exit = true;
            }
            imgui::PopStyleVar();
            imgui::EndMenu();
        }

        // Edit
        if (imgui::BeginMenu("Edit")) {
            imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (imgui::MenuItem("Undo")) {
            }
            if (imgui::MenuItem("Redo")) {
            }
            imgui::PopStyleVar();
            imgui::EndMenu();
        }

        // View
        if (imgui::BeginMenu("View")) {
            imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

            if (imgui::BeginMenu("Features")) {
                imgui::Checkbox("Wireframe", &render_layer->render_wireframe);
                imgui::Checkbox("vSync", &render_layer->v_sync);
                imgui::EndMenu();
            }
            if (imgui::BeginMenu("Values")) {
                imgui::SetNextItemWidth(75.0f);
                imgui::DragInt("Outline Size", &editor_layer->outline_size);
                imgui::EndMenu();
            }
            if (imgui::BeginMenu("Colors")) {
                imgui::SetNextItemWidth(250.0f);
                imgui::ColorEdit3("Special", gui_layer->special_color);
                imgui::SetNextItemWidth(250.0f);
                imgui::ColorEdit3("Alternate", gui_layer->alternate_color);
                imgui::EndMenu();
            }

            imgui::PopStyleVar();
            imgui::EndMenu();
        }

        // About
        if (imgui::BeginMenu("About")) {
            imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            imgui::Text("Titian 0.64v");
            imgui::Text(kl::format("Serial ", SERIAL_VERSION_FORMAT).c_str());
            imgui::PopStyleVar();
            imgui::EndMenu();
        }

        // Game control
        imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 5.0f));
        imgui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        imgui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        imgui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(30, 30, 30));
        imgui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        imgui::SetCursorPosX((menu_size.x - m_control_buttons_width) * 0.5f);
        m_control_buttons_width = 0.0f;

        const float button_image_size = 16.0f;
        
        const bool is_start_enabled = !game_layer->game_running || game_layer->game_paused;
        const ImVec4 start_tint_color = is_start_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        imgui::PushItemFlag(ImGuiItemFlags_Disabled, !is_start_enabled);
        if (imgui::ImageButton(m_start_button_texture->shader_view.Get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), start_tint_color)) {
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
        m_control_buttons_width += imgui::GetItemRectSize().x;
        imgui::PopItemFlag();

        const bool is_pause_enabled = game_layer->game_running && !game_layer->game_paused;
        const ImVec4 pause_tint_color = is_pause_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        imgui::PushItemFlag(ImGuiItemFlags_Disabled, !is_pause_enabled);
        if (imgui::ImageButton(m_pause_button_texture->shader_view.Get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), pause_tint_color)) {
            game_layer->pause_game();
        }
        m_control_buttons_width += imgui::GetItemRectSize().x;
        imgui::PopItemFlag();

        const bool is_stop_enabled = game_layer->game_running || game_layer->game_paused;
        const ImVec4 stop_tint_color = is_stop_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        imgui::PushItemFlag(ImGuiItemFlags_Disabled, !is_stop_enabled);
        if (imgui::ImageButton(m_stop_button_texture->shader_view.Get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), stop_tint_color)) {
            game_layer->stop_game();
            if (const Serializer serializer = { m_temp_path, false }) {
                game_layer->reset_scene();
                game_layer->scene->deserialize(&serializer, nullptr);
            }
        }
        m_control_buttons_width += imgui::GetItemRectSize().x;
        imgui::PopItemFlag();

        imgui::PopStyleColor(2);
        imgui::PopStyleVar(3);

#if 0
        // Gizmo control
        imgui::SetCursorPosX(menu_size.x - m_gizmo_buttons_width);
        m_gizmo_buttons_width = 0.0f;

        if (imgui::BeginMenu("Gizmos")) {
            imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

            bool scaling_state = editor_layer->gizmo_operation == ImGuizmo::SCALE;
            if (imgui::Checkbox("Scaling", &scaling_state)) {
                editor_layer->gizmo_operation = scaling_state ? ImGuizmo::SCALE : 0;
            }
            bool rotation_state = editor_layer->gizmo_operation == ImGuizmo::ROTATE;
            if (imgui::Checkbox("Rotation", &rotation_state)) {
                editor_layer->gizmo_operation = rotation_state ? ImGuizmo::ROTATE : 0;
            }
            bool translation_state = editor_layer->gizmo_operation == ImGuizmo::TRANSLATE;
            if (imgui::Checkbox("Translation", &translation_state)) {
                editor_layer->gizmo_operation = translation_state ? ImGuizmo::TRANSLATE : 0;
            }

            imgui::PopStyleVar();
            imgui::EndMenu();
        }
        m_gizmo_buttons_width += imgui::GetItemRectSize().x;
#endif

        imgui::EndMainMenuBar();
    }

    imgui::PopStyleColor();
    imgui::PopStyleVar(2);
}
