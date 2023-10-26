export module section_material_editor;

export import gui_section;
export import editor_layer;
export import gui_layer;

export namespace titian {
    class GUISectionMaterialEditor : public GUISection
    {
    public:
        kl::Object<EditorLayer> editor_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;

        kl::Object<Camera> camera = nullptr;
        kl::Object<Texture> render_texture = nullptr;
        kl::Object<Texture> depth_texture = nullptr;
        kl::Float4 line_color = kl::colors::WHITE;

        bool was_focused = true;
        int last_scroll = 0;
        kl::Float2 initial_camera_info = { 40, 30 };
        kl::Float2 camera_info = initial_camera_info;

        std::string selected_material = {};
        std::string selected_texture = {};

        GUISectionMaterialEditor(kl::Object<EditorLayer>& editor_layer, kl::Object<GUILayer>& gui_layer)
        {
            this->editor_layer = editor_layer;
            this->gui_layer = gui_layer;

            kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
            Scene* scene = &editor_layer->game_layer->scene;

            camera = new Camera(scene->physics(), true);
            render_texture = new Texture(gpu);
            depth_texture = new Texture(gpu);

            camera->background = kl::Color{ 30, 30, 30 };
            camera->set_position({ 0.642787576f, 0.577350259f, 0.766044438f });
            camera->speed = 3.1f;
        }

        ~GUISectionMaterialEditor() override
        {}

        void render_gui() override
        {
            kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
            Scene* scene = &editor_layer->game_layer->scene;
            Material* material = &scene->get_material(this->selected_material);
            Texture* texture = &scene->get_texture(this->selected_texture);

            if (ImGui::Begin("Material Editor")) {
                const float available_width = ImGui::GetContentRegionAvail().x;
                ImGui::Columns(3, "MaterialEditorColumns", false);

                ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
                if (ImGui::BeginChild("Materials")) {
                    display_materials(scene);
                }
                ImGui::EndChild();

                ImGui::NextColumn();
                ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
                if (ImGui::BeginChild("Textures")) {
                    display_textures(scene);
                }
                ImGui::EndChild();

                const std::optional texture_file_path = gui_get_drag_drop<std::string>("TextureFile");
                if (texture_file_path) {
                    const std::filesystem::path path = texture_file_path.value();
                    const std::string texture_name = path.filename().string();
                    if (!scene->textures.contains(texture_name)) {
                        kl::Object new_texture = new Texture(gpu);
                        new_texture->data_buffer.load_from_file(path.string());
                        new_texture->load_as_2D(false, false);
                        new_texture->create_shader_view(nullptr);
                        scene->textures[texture_name] = new_texture;
                    }
                }
                ImGui::NextColumn();

                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

                const bool should_rotate_cam = was_focused && !texture;
                if (ImGui::BeginChild("Material/Texture View", {}, should_rotate_cam)) {
                    const kl::Int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
                    if (should_rotate_cam) {
                        update_material_camera();
                    }
                    if (material) {
                        render_selected_material(scene, gpu, material, viewport_size);
                        ImGui::Image(render_texture->shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
                    }
                    else if (texture) {
                        render_selected_texture(texture, viewport_size);
                    }
                    was_focused = ImGui::IsWindowFocused();
                }
                ImGui::EndChild();

                ImGui::PopStyleColor();
                ImGui::PopStyleVar(2);

                if (material) {
                    show_material_properties(scene, material);
                }
                else if (texture) {
                    show_texture_properties(texture);
                }
            }
            ImGui::End();
        }

    private:
        void display_materials(Scene* scene)
        {
            if (ImGui::BeginPopupContextWindow("CreateMaterial", ImGuiPopupFlags_MouseButtonRight)) {
                ImGui::Text("Create Material");

                if (std::optional name = gui_input_waited("##CreateMaterialInput", {})) {
                    if (!scene->materials.contains(name.value())) {
                        kl::Object material = new Material();
                        scene->materials[name.value()] = material;
                        ImGui::CloseCurrentPopup();
                    }
                }
                ImGui::EndPopup();
            }

            const std::string filter = gui_input_continuous("Search###MeterialEditor");
            for (const auto& [material_name, material] : scene->materials) {
                if (!filter.empty() && !material_name.contains(filter)) {
                    continue;
                }

                if (ImGui::Selectable(material_name.c_str(), material_name == this->selected_material)) {
                    this->selected_material = material_name;
                    this->selected_texture = "/";
                }

                if (ImGui::BeginPopupContextItem(material_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
                    bool should_break = false;
                    ImGui::Text("Edit Material");

                    if (std::optional name = gui_input_waited("##RenameMaterialInput", material_name)) {
                        if (!scene->materials.contains(name.value())) {
                            for (auto& [_, entity] : *scene) {
                                if (entity->material_name == material_name) {
                                    entity->material_name = name.value();
                                }
                            }
                            if (this->selected_material == material_name) {
                                this->selected_material = name.value();
                            }
                            scene->materials[name.value()] = material;
                            scene->materials.erase(material_name);
                            should_break = true;
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                        for (auto& [_, entity] : *scene) {
                            if (entity->material_name == material_name) {
                                entity->material_name = "/";
                            }
                        }
                        if (this->selected_material == material_name) {
                            selected_material = "/";
                        }
                        scene->materials.erase(material_name);
                        should_break = true;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                    if (should_break) {
                        break;
                    }
                }
            }
        }

        void display_textures(Scene* scene)
        {
            const std::string filter = gui_input_continuous("Search###TextureEditor");
            for (const auto& [texture_name, texture] : scene->textures) {
                if (!filter.empty() && !texture_name.contains(filter)) {
                    continue;
                }

                if (ImGui::Selectable(texture_name.c_str(), texture_name == this->selected_texture)) {
                    this->selected_texture = texture_name;
                    this->selected_material = "/";
                }

                if (ImGui::BeginPopupContextItem(texture_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
                    bool should_break = false;
                    ImGui::Text("Edit Texture");

                    if (std::optional name = gui_input_waited("##RenameTextureInput", texture_name)) {
                        if (!scene->textures.contains(name.value())) {
                            for (auto& [_, material] : scene->materials) {
                                if (material->color_map_name == texture_name) {
                                    material->color_map_name = name.value();
                                }
                                if (material->normal_map_name == texture_name) {
                                    material->normal_map_name = name.value();
                                }
                                if (material->roughness_map_name == texture_name) {
                                    material->roughness_map_name = name.value();
                                }
                            }
                            if (this->selected_texture == texture_name) {
                                this->selected_texture = name.value();
                            }
                            scene->textures[name.value()] = texture;
                            scene->textures.erase(texture_name);
                            should_break = true;
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                        for (auto& [_, material] : scene->materials) {
                            if (material->color_map_name == texture_name) {
                                material->color_map_name = "/";
                            }
                            if (material->normal_map_name == texture_name) {
                                material->normal_map_name = "/";
                            }
                            if (material->roughness_map_name == texture_name) {
                                material->roughness_map_name = "/";
                            }
                        }
                        if (this->selected_texture == texture_name) {
                            this->selected_texture = "/";
                        }
                        scene->textures.erase(texture_name);
                        should_break = true;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                    if (should_break) {
                        break;
                    }
                }
            }
        }

        void update_material_camera()
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                initial_camera_info = camera_info;
            }

            const int scroll = editor_layer->game_layer->app_layer->window->mouse.scroll();
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                const ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
                camera_info.x = initial_camera_info.x + drag_delta.x * camera->sensitivity;
                camera_info.y = initial_camera_info.y + drag_delta.y * camera->sensitivity;
                camera_info.y = kl::clamp(camera_info.y, -85.0f, 85.0f);

                camera->set_position({
                    kl::sin_deg(camera_info.x),
                    kl::tan_deg(camera_info.y),
                    kl::cos_deg(camera_info.x),
                });

                camera->speed += (last_scroll - scroll) * 0.1f;
                camera->speed = std::max(camera->speed, 0.1f);
            }
            last_scroll = scroll;

            const float camera_distance = camera->speed;
            camera->set_position(kl::normalize(camera->position()) * camera_distance);
            camera->set_forward(-camera->position());
        }

        void render_selected_material(Scene* scene, kl::GPU* gpu, const Material* material, const kl::Int2 viewport_size)
        {
            if (viewport_size.x <= 0 || viewport_size.y <= 0) {
                return;
            }

            if (render_texture->graphics_buffer_size() != viewport_size) {
                render_texture->graphics_buffer = gpu->create_target_texture(viewport_size);
                render_texture->create_target_view(nullptr);
                render_texture->create_shader_view(nullptr);
            }
            if (depth_texture->graphics_buffer_size() != viewport_size) {
                kl::dx::TextureDescriptor descriptor = {};
                descriptor.Width = viewport_size.x;
                descriptor.Height = viewport_size.y;
                descriptor.MipLevels = 1;
                descriptor.ArraySize = 1;
                descriptor.Format = DXGI_FORMAT_D32_FLOAT;
                descriptor.SampleDesc.Count = 1;
                descriptor.Usage = D3D11_USAGE_DEFAULT;
                descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                depth_texture->graphics_buffer = gpu->create_texture(&descriptor, nullptr);
                depth_texture->create_depth_view(nullptr);
            }

            gpu->bind_target_depth_views({ render_texture->target_view }, depth_texture->depth_view);
            gpu->clear_target_view(render_texture->target_view, camera->background);
            gpu->clear_depth_view(depth_texture->depth_view, 1.0f, 0xFF);

            const kl::Int2 old_viewport_size = gpu->viewport_size();
            gpu->set_viewport_size(viewport_size);

            RenderStates* states = &gui_layer->render_layer->states;
            gpu->bind_raster_state(states->raster_states->solid);
            gpu->bind_depth_state(states->depth_states->enabled);

            kl::RenderShaders& render_shaders = states->shader_states->material_pass;
            gpu->bind_render_shaders(render_shaders);

            camera->update_aspect_ratio(viewport_size);

            class VSData
            {
            public:
                kl::Float4x4  w_matrix;
                kl::Float4x4 vp_matrix;
            } vs_data = {};
            vs_data.w_matrix = {};
            vs_data.vp_matrix = camera->camera_matrix();

            class PSData
            {
            public:
                kl::Float4 object_color; // (color.r, color.g, color.b, none)

                kl::Float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
                kl::Float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)

                kl::Float4 camera_info; // (camera.x, camera.y, camera.z, skybox?)
                kl::Float4 camera_background; // (color.r, color.g, color.b, color.a)

                kl::Float4     ambient_light; // (color.r, color.g, color.b, intensity)
                kl::Float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)
            } ps_data = {};

            Camera* scene_camera = scene->get_casted<Camera>(scene->main_camera_name);
            if (!scene_camera) {
                return;
            }

            Texture* skybox_texture = &scene->get_texture(scene_camera->skybox_name);
            if (skybox_texture) {
                gpu->bind_shader_view_for_pixel_shader(skybox_texture->shader_view, 0);
            }
            else {
                gpu->unbind_shader_view_for_pixel_shader(0);
            }

            Texture* color_map_texture = &scene->get_texture(material->color_map_name);
            if (color_map_texture) {
                gpu->bind_shader_view_for_pixel_shader(color_map_texture->shader_view, 1);
            }
            else {
                gpu->unbind_shader_view_for_pixel_shader(1);
            }

            Texture* normal_map_texture = &scene->get_texture(material->normal_map_name);
            if (normal_map_texture) {
                gpu->bind_shader_view_for_pixel_shader(normal_map_texture->shader_view, 2);
                ps_data.object_texture_info.x = 1.0f;
            }
            else {
                ps_data.object_texture_info.x = 0.0f;
            }

            Texture* roughness_map_texture = &scene->get_texture(material->roughness_map_name);
            if (roughness_map_texture) {
                gpu->bind_shader_view_for_pixel_shader(roughness_map_texture->shader_view, 3);
                ps_data.object_texture_info.y = 1.0f;
            }
            else {
                ps_data.object_texture_info.y = 0.0f;
            }

            ps_data.ambient_light = { kl::Float3 {1.0f}, 0.1f };
            ps_data.directional_light = { kl::normalize(kl::Float3 { 0.0f, -1.0f, -1.0f }), 1.0f };

            ps_data.object_color = material->color;
            ps_data.object_material = {
                material->texture_blend,
                material->reflection_factor,
                material->refraction_factor,
                material->refraction_index,
            };
            ps_data.camera_info = { camera->position(), static_cast<float>(static_cast<bool>(skybox_texture)) };
            ps_data.camera_background = camera->background;

            render_shaders.vertex_shader.update_cbuffer(vs_data);
            render_shaders.pixel_shader.update_cbuffer(ps_data);

            DefaultMeshes* default_meshes = &editor_layer->game_layer->scene->default_meshes;
            gpu->draw(default_meshes->cube->graphics_buffer);

            gpu->bind_internal_views();
            gpu->set_viewport_size(old_viewport_size);
        }

        void render_selected_texture(Texture* texture, const kl::Int2 viewport_size)
        {
            const float min_size = static_cast<float>(std::min(viewport_size.x, viewport_size.y));
            ImGui::SetCursorPos(ImVec2{
                (ImGui::GetWindowWidth() - min_size) * 0.5f,
                (ImGui::GetWindowHeight() - min_size) * 0.5f,
            });
            ImGui::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
        }

        void show_material_properties(Scene* scene, Material* material)
        {
            if (ImGui::Begin("Material Properties") && material) {
                ImGui::Text("Info");

                ImGui::Text("Name: ");
                ImGui::SameLine();
                gui_colored_text(selected_material, gui_layer->special_color);

                ImGui::DragFloat("Texture Blend", &material->texture_blend, 0.05f, 0.0f, 1.0f);
                ImGui::DragFloat("Reflection Factor", &material->reflection_factor, 0.05f, 0.0f, 1.0f);
                ImGui::DragFloat("Refraction Factor", &material->refraction_factor, 0.05f, 0.0f, 1.0f);
                ImGui::DragFloat("Refraction Index", &material->refraction_index, 0.05f, 0.0f, 1.0f);

                ImGui::ColorEdit4("Base Color", material->color);

                if (ImGui::BeginCombo("Color Map", material->color_map_name.c_str())) {
                    if (ImGui::Selectable("/", material->color_map_name == "/")) {
                        material->color_map_name = "/";
                    }
                    for (auto& [texture_name, _] : scene->textures) {
                        if (ImGui::Selectable(texture_name.c_str(), texture_name == material->color_map_name)) {
                            material->color_map_name = texture_name;
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::BeginCombo("Normal Map", material->normal_map_name.c_str())) {
                    if (ImGui::Selectable("/", material->normal_map_name == "/")) {
                        material->normal_map_name = "/";
                    }
                    for (auto& [texture_name, _] : scene->textures) {
                        if (ImGui::Selectable(texture_name.c_str(), texture_name == material->normal_map_name)) {
                            material->normal_map_name = texture_name;
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::BeginCombo("Roughness Map", material->roughness_map_name.c_str())) {
                    if (ImGui::Selectable("/", material->roughness_map_name == "/")) {
                        material->roughness_map_name = "/";
                    }
                    for (auto& [texture_name, _] : scene->textures) {
                        if (ImGui::Selectable(texture_name.c_str(), material->roughness_map_name == texture_name)) {
                            material->roughness_map_name = texture_name;
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::End();
        }

        void show_texture_properties(Texture* texture)
        {
            if (ImGui::Begin("Texture Properties") && texture) {
                ImGui::Text("Info");

                ImGui::Text("Name: ");
                ImGui::SameLine();
                gui_colored_text(selected_texture, gui_layer->special_color);

                kl::Int2 size = texture->data_buffer.size();
                ImGui::DragInt2("Size", size, 0.0f);

                int pixel_count = texture->data_buffer.pixel_count();
                ImGui::DragInt("Pixel Count", &pixel_count, 0.0f);

                bool cube_map = texture->is_cube();
                if (ImGui::Checkbox("Cube Map", &cube_map)) {
                    if (cube_map) {
                        if (texture->load_as_cube()) {
                            texture->create_shader_view(nullptr);
                        }
                    }
                    else {
                        texture->load_as_2D(false, false);
                        texture->create_shader_view(nullptr);
                    }
                }
            }
            ImGui::End();
        }
    };
}
