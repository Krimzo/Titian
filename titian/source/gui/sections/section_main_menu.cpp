#include "titian.h"


titian::GUISectionMainMenu::GUISectionMainMenu()
    : GUISection("GUISectionMainMenu")
{
    const auto create_texture = [&](Ref<Texture>& texture, const char* filename)
    {
        texture = new Texture();
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D();
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(m_start_button_texture, "package/textures/start_button.png"); });
    queue.add_task([&] { create_texture(m_pause_button_texture, "package/textures/pause_button.png"); });
    queue.add_task([&] { create_texture(m_stop_button_texture, "package/textures/stop_button.png"); });
    queue.finalize();
}

void titian::GUISectionMainMenu::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

	AppLayer* app_layer = Layers::get<AppLayer>();
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    GameLayer* game_layer = Layers::get<GameLayer>();
    GUILayer* gui_layer = Layers::get<GUILayer>();
    EditorLayer* editor_layer = Layers::get<EditorLayer>();

    Ref<Scene>& scene = game_layer->scene;

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
        const ImVec2 menu_size = im::GetContentRegionAvail();

        if (im::BeginMenu("File")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::BeginMenu("Import")) {
                if (im::MenuItem("Mesh")) {
                    if (auto file = kl::choose_file(false, { { "Mesh File",  FILE_EXTENSION_OBJ }, { "Mesh File",  FILE_EXTENSION_GLB }, { "Mesh File",  FILE_EXTENSION_FBX } })) {
                        if (auto data = scene->get_assimp_data(file.value())) {
                            const aiScene* ai_scene = data.value().importer->GetScene();
                            for (uint32_t i = 0; i < ai_scene->mNumMeshes; i++) {
                                Ref mesh = scene->load_assimp_mesh(ai_scene, ai_scene->mMeshes[i]);
								scene->meshes[data.value().meshes[i]] = mesh;
                            }
                        }
                    }
                }
                if (im::MenuItem("Texture")) {
                    if (auto file = kl::choose_file(false, { { "Texture File", FILE_EXTENSION_JPG }, { "Texture File", FILE_EXTENSION_PNG } })) {
                        const String stem_name = fs::path(file.value()).stem().string();

                        Texture* texture = scene->helper_new_texture(stem_name);
                        texture->data_buffer.load_from_file(file.value());
                        texture->reload_as_2D();
                        texture->create_shader_view();
                    }
                }
                if (im::MenuItem("Script")) {
                    int type_index = 0;
                    if (auto file = kl::choose_file(false, { { "Script File", FILE_EXTENSION_LUA }, { "Script File", FILE_EXTENSION_JSON }, { "Script File", FILE_EXTENSION_DLL } }, &type_index)) {
                        const String stem_name = fs::path(file.value()).stem().string();

                        if (type_index == 0) {
                            Ref script = new NativeScript();
                            script->data = kl::read_file(file.value());
							script->reload();
                            scene->scripts[stem_name] = script;
                        }
                        else if (type_index == 1) {
                            Ref script = new NodeScript();
                            if (const TextSerializer serializer{ file.value(), false }) {
                                String ignored_type;
                                serializer.read_string("script_type", ignored_type);
                                script->deserialize(&serializer);
                            }
                            script->reload();
                            scene->scripts[stem_name] = script;
                        }
                        else if (type_index == 2) {
                            Ref script = new InterpScript();
                            script->source = kl::read_file(file.value());
                            script->reload();
                            scene->scripts[stem_name] = script;
                        }
                    }
                }
                if (im::MenuItem("Shader")) {
                    if (auto file = kl::choose_file(false, { { "Shader File", FILE_EXTENSION_HLSL } })) {
                        const String stem_name = fs::path(file.value()).stem().string();

                        Shader* shader = scene->helper_new_shader(stem_name);
                        shader->data_buffer = kl::read_file(file.value());
                    }
                }
                if (im::BeginMenu("Scene")) {
                    if (im::MenuItem("Binary")) {
                        if (auto file = kl::choose_file(false, { { "Scene File", FILE_EXTENSION_TITIAN } })) {
                            const String stem_name = fs::path(file.value()).stem().string();
                            if (const BinarySerializer serializer{ file.value(), false }) {
                                Ref scene = new Scene();
                                scene->deserialize(&serializer);
                                game_layer->scene = scene;
                            }
                        }
                    }
                    if (im::MenuItem("Text")) {
                        if (auto file = kl::choose_file(false, { { "Scene File", FILE_EXTENSION_JSON } })) {
                            const String stem_name = fs::path(file.value()).stem().string();
                            if (const TextSerializer serializer{ file.value(), false }) {
                                Ref scene = new Scene();
                                scene->deserialize(&serializer);
                                game_layer->scene = scene;
                            }
                        }
                    }
                    im::EndMenu();
                }
                im::EndMenu();
            }
            if (im::BeginMenu("Export")) {
                if (im::BeginMenu("Texture")) {
                    const String filter = gui_input_continuous("Search###MenuExportTexture");
                    for (auto& [name, texture] : scene->textures) {
                        if (!filter.empty() && name.find(filter) == -1) {
                            continue;
                        }
                        if (im::MenuItem(name.data())) {
                            int type_index = 0;
                            if (auto file = kl::choose_file(true, { { "Texture File",  FILE_EXTENSION_JPG }, { "Texture File",  FILE_EXTENSION_PNG } }, &type_index)) {
                                const String extension = fs::path(file.value()).extension().string();
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
                    im::EndMenu();
                }
                if (im::BeginMenu("Script")) {
                    const String filter = gui_input_continuous("Search###MenuExportScript");
                    for (auto& [name, script] : scene->scripts) {
                        if (!filter.empty() && name.find(filter) == -1) {
                            continue;
                        }
                        if (im::MenuItem(name.data())) {
                            Pair<String, String> type_info;
                            if (script.is<InterpScript>()) {
                                type_info = Pair{ "Interp Script", FILE_EXTENSION_LUA };
                            }
                            else if (script.is<NodeScript>()) {
                                type_info = Pair{ "Node Script", FILE_EXTENSION_JSON };
                            }
                            else {
                                type_info = Pair{ "Native Script", FILE_EXTENSION_DLL };
                            }
                            if (auto file = kl::choose_file(true, { type_info })) {
                                const String extension = fs::path(file.value()).extension().string();
                                if (InterpScript* interp_script = &script.as<InterpScript>()) {
                                    if (extension.empty()) {
                                        file.value() += FILE_EXTENSION_LUA;
                                    }
                                    kl::write_file(file.value(), interp_script->source);
                                }
                                else if (NodeScript* node_script = &script.as<NodeScript>()) {
                                    if (extension.empty()) {
                                        file.value() += FILE_EXTENSION_JSON;
                                    }
                                    if (TextSerializer serializer{ file.value(), true }) {
                                        node_script->serialize(&serializer);
                                    }
                                }
                                else if (NativeScript* native_script = &script.as<NativeScript>()) {
                                    if (extension.empty()) {
                                        file.value() += FILE_EXTENSION_DLL;
                                    }
									kl::write_file(file.value(), native_script->data);
                                }
                            }
                        }
                    }
                    im::EndMenu();
                }
                if (im::BeginMenu("Shader")) {
                    const String filter = gui_input_continuous("Search###MenuExportShader");
                    for (auto& [name, shader] : scene->shaders) {
                        if (!filter.empty() && name.find(filter) == -1) {
                            continue;
                        }
                        if (im::MenuItem(name.data())) {
                            if (auto file = kl::choose_file(true, { { "Shader File", FILE_EXTENSION_HLSL } })) {
                                const String extension = fs::path(file.value()).extension().string();
                                if (extension.empty()) {
                                    file.value() += FILE_EXTENSION_HLSL;
                                }
                                kl::write_file(file.value(), shader->data_buffer);
                            }
                        }
                    }
                    im::EndMenu();
                }
                if (im::BeginMenu("Scene")) {
                    if (im::MenuItem("Binary")) {
                        if (auto file = kl::choose_file(true, { { "Scene File", FILE_EXTENSION_TITIAN } })) {
                            const String extension = fs::path(file.value()).extension().string();
                            if (extension.empty()) {
                                file.value() += FILE_EXTENSION_TITIAN;
                            }
                            if (BinarySerializer serializer{ file.value(), true }) {
                                scene->serialize(&serializer);
                            }
                        }
                    }
                    if (im::MenuItem("Text")) {
                        if (auto file = kl::choose_file(true, { { "Scene File", FILE_EXTENSION_JSON } })) {
                            const String extension = fs::path(file.value()).extension().string();
                            if (extension.empty()) {
                                file.value() += FILE_EXTENSION_JSON;
                            }
                            if (TextSerializer serializer{ file.value(), true }) {
                                scene->serialize(&serializer);
                            }
                        }
                    }
                    im::EndMenu();
                }
                im::EndMenu();
            }
            im::Separator();
            if (im::MenuItem("New Scene")) {
                scene = new Scene();
            }
            im::Separator();
            if (im::MenuItem("Exit")) {
                m_testing_exit = true;
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        if (im::BeginMenu("Edit")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::MenuItem("Undo")) {
            }
            if (im::MenuItem("Redo")) {
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        static Optional<AssimpData> opt_assimp_data;
        if (im::BeginMenu("Tools")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::MenuItem("Import Packed")) {
                if (auto file_path = kl::choose_file(false, { { "GLTF File", FILE_EXTENSION_GLB }, { "FBX File", FILE_EXTENSION_FBX } })) {
                    opt_assimp_data = scene->get_assimp_data(file_path.value());
                }
            }
            if (im::MenuItem("Quick Setup")) {
                Ref camera = new Camera();
                camera->set_position(Float3{ 1.5f });
                camera->set_forward(-camera->position());
                scene->add_entity("camera", camera);
                scene->main_camera_name = "camera";

                Ref ambient = new AmbientLight();
                scene->add_entity("ambient", ambient);
                scene->main_ambient_light_name = "ambient";

                Ref sun = new DirectionalLight();
                sun->set_direction({ -0.666f, -0.666f, 0.333f });
                scene->add_entity("sun", sun);
                scene->main_directional_light_name = "sun";

                Ref cube = new Entity();
                cube->animation_name = "cube";
                cube->material_name = "white";
                scene->add_entity("cube", cube);

                Ref mesh = new Mesh();
                mesh->load_triangles(kl::GPU::generate_cube_mesh(1.0f));
                scene->meshes["cube"] = mesh;

                Ref animation = new Animation();
                animation->meshes = { "cube" };
                scene->animations["cube"] = animation;

                Ref material = new Material();
                material->color = kl::colors::WHITE;
                scene->materials["white"] = material;
            }
            im::PopStyleVar();
            im::EndMenu();
        }
        if (opt_assimp_data) {
            const auto& assimp_data = opt_assimp_data.value();
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
            if (im::Begin("GLTF Import")) {
                im::Text("Meshes:");
                for (size_t i = 0; i < assimp_data.meshes.size(); i++) {
                    im::Text(kl::format(i + 1, ". \"", assimp_data.meshes[i], "\"").data());
                }
                im::Separator();

                im::Text("Animations:");
                for (size_t i = 0; i < assimp_data.animations.size(); i++) {
                    im::Text(kl::format(i + 1, ". \"", assimp_data.animations[i], "\"").data());
                }
                im::Separator();

                im::Text("Textures:");
                for (size_t i = 0; i < assimp_data.textures.size(); i++) {
                    im::Text(kl::format(i + 1, ". \"", assimp_data.textures[i], "\"").data());
                }
                im::Separator();

                im::Text("Materials:");
                for (size_t i = 0; i < assimp_data.materials.size(); i++) {
                    im::Text(kl::format(i + 1, ". \"", assimp_data.materials[i], "\"").data());
                }
                im::Separator();

                im::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7.5f, 7.5f));
                if (im::Button("Cancel")) {
                    opt_assimp_data = std::nullopt;
                }
                im::SameLine();
                if (im::Button("Import")) {
                    scene->load_assimp_data(assimp_data);
                    opt_assimp_data = std::nullopt;
                }
                im::PopStyleVar();
            }
            im::End();
            im::PopStyleVar();
        }

        if (im::BeginMenu("View")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

            if (im::BeginMenu("Values")) {
                im::SetNextItemWidth(75.0f);
                im::DragInt("Outline Size", &editor_layer->outline_size);
                im::EndMenu();
            }
            if (im::BeginMenu("Colors")) {
                im::SetNextItemWidth(250.0f);
                im::ColorEdit3("Special", &gui_layer->special_color.x);
                im::SetNextItemWidth(250.0f);
                im::ColorEdit3("Alternate", &gui_layer->alternate_color.x);
                if (im::Button("Reload", { -1.0f, 0.0f })) {
                    gui_layer->reload_colors();

                    const Color special_color = gui_layer->special_color;
                    kl::Wrap special_wrap = kl::Wrap<js::Array>::make();
                    special_wrap->push_back(js::make_number(special_color.r));
                    special_wrap->push_back(js::make_number(special_color.g));
                    special_wrap->push_back(js::make_number(special_color.b));
                    _conf_data[CONF_SPECIAL_COLOR] = std::move(special_wrap);

                    const Color alternate_color = gui_layer->alternate_color;
                    kl::Wrap alternate_wrap = kl::Wrap<js::Array>::make();
                    alternate_wrap->push_back(js::make_number(alternate_color.r));
                    alternate_wrap->push_back(js::make_number(alternate_color.g));
                    alternate_wrap->push_back(js::make_number(alternate_color.b));
                    _conf_data[CONF_ALTERNATE_COLOR] = std::move(alternate_wrap);

                    kl::write_file(_CONF_FILE, _conf_data.decompile());
                }
                im::EndMenu();
            }

            im::PopStyleVar();
            im::EndMenu();
        }

        if (im::BeginMenu("About")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            im::Text("Titian 0.67v");
            im::Text(kl::format("Serial ", SERIAL_VERSION_STR).data());
            im::PopStyleVar();
            im::EndMenu();
        }

        im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 5.0f));
        im::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        im::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        im::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(30, 30, 30));
        im::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        im::SetCursorPosX((menu_size.x - m_control_buttons_width) * 0.5f);
        m_control_buttons_width = 0.0f;

        constexpr float button_image_size = 16.0f;
        
        const bool is_start_enabled = !game_layer->game_running || game_layer->game_paused;
        const ImVec4 start_tint_color = is_start_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        im::PushItemFlag(ImGuiItemFlags_Disabled, !is_start_enabled);
        if (im::ImageButton("StartButton", m_start_button_texture->shader_view.get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), start_tint_color)) {
            if (game_layer->game_paused) {
                game_layer->resume_game();
            }
            else {
                if (BinarySerializer serializer{ m_temp_path, true }) {
                    game_layer->scene->serialize(&serializer);
                }
                game_layer->start_game();
            }
        }
        m_control_buttons_width += im::GetItemRectSize().x;
        im::PopItemFlag();

        const bool is_pause_enabled = game_layer->game_running && !game_layer->game_paused;
        const ImVec4 pause_tint_color = is_pause_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        im::PushItemFlag(ImGuiItemFlags_Disabled, !is_pause_enabled);
        if (im::ImageButton("PauseButton", m_pause_button_texture->shader_view.get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), pause_tint_color)) {
            game_layer->pause_game();
        }
        m_control_buttons_width += im::GetItemRectSize().x;
        im::PopItemFlag();

        const bool is_stop_enabled = game_layer->game_running || game_layer->game_paused;
        const ImVec4 stop_tint_color = is_stop_enabled ? ImColor(200, 200, 200) : ImColor(75, 75, 75);

        im::PushItemFlag(ImGuiItemFlags_Disabled, !is_stop_enabled);
        if (im::ImageButton("StopButton", m_stop_button_texture->shader_view.get(), ImVec2(button_image_size, button_image_size), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), stop_tint_color)) {
            game_layer->stop_game();
            if (const BinarySerializer serializer{ m_temp_path, false }) {
                game_layer->reset_scene();
                game_layer->scene->deserialize(&serializer);
            }
        }
        m_control_buttons_width += im::GetItemRectSize().x;
        im::PopItemFlag();

        im::PopStyleColor(2);
        im::PopStyleVar(3);

        im::EndMainMenuBar();
    }

    im::PopStyleColor();
    im::PopStyleVar(2);
}
