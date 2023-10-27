export module section_mesh_editor;

export import gui_section;
export import editor_layer;
export import gui_layer;

export namespace titian {
    class GUISectionMeshEditor : public GUISection
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        kl::Object<Camera> camera = nullptr;
        kl::Object<Texture> render_texture = nullptr;
        kl::Float4 line_color = kl::colors::WHITE;

        bool was_focused = true;
        int last_scroll = 0;
        kl::Float2 initial_camera_info = { 40.0f, 30.0f };
        kl::Float2 camera_info = initial_camera_info;

        std::string selected_mesh = "/";

        GUISectionMeshEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
        {
            this->editor_layer = editor_layer;
            this->gui_layer = gui_layer;
            
            kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
            Scene* scene = &editor_layer->game_layer->scene;

            camera = new Camera(scene->physics(), true);
            render_texture = new Texture(gpu);

            camera->background = kl::Color{ 30, 30, 30 };
            camera->set_position({ 0.642787576f, 0.577350259f, 0.766044438f });
            camera->speed = 3.1f;
        }

        ~GUISectionMeshEditor() override
        {}

        void render_gui() override
        {
            kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
            Scene* scene = &editor_layer->game_layer->scene;
            Mesh* mesh = &scene->get_mesh(this->selected_mesh);

            if (ImGui::Begin("Mesh Editor")) {
                const float available_width = ImGui::GetContentRegionAvail().x;
                ImGui::Columns(2, "MeshEditorColumns", false);

                ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
                if (ImGui::BeginChild("Meshes")) {
                    display_meshes(scene);
                }
                ImGui::EndChild();

                const std::optional mesh_file_path = gui_get_drag_drop<std::string>("MeshFile");
                if (mesh_file_path) {
                    const std::filesystem::path path = mesh_file_path.value();
                    const std::string mesh_name = path.filename().string();
                    if (!scene->meshes.contains(mesh_name)) {
                        kl::Object<Mesh> new_mesh = new Mesh(gpu, scene->physics(), scene->cooking());
                        new_mesh->data_buffer = kl::parse_obj_file(path.string());
                        new_mesh->reload();
                        scene->meshes[mesh_name] = new_mesh;
                    }
                }
                ImGui::NextColumn();

                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

                if (ImGui::BeginChild("Mesh View", {}, was_focused)) {
                    const kl::Int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
                    if (was_focused) {
                        update_mesh_camera();
                    }
                    if (mesh) {
                        render_selected_mesh(gpu, mesh, viewport_size);
                        const kl::dx::ShaderView& shader_view = render_texture->shader_view;
                        ImGui::Image(render_texture->shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
                    }
                    was_focused = ImGui::IsWindowFocused();
                }
                ImGui::EndChild();

                ImGui::PopStyleColor();
                ImGui::PopStyleVar(2);

                show_mesh_properties(mesh);
            }
            ImGui::End();
        }

    private:
        void display_meshes(Scene* scene)
        {
            const std::string filter = gui_input_continuous("Search###MeshEditor");
            for (const auto& [mesh_name, mesh] : scene->meshes) {
                if (!filter.empty() && !mesh_name.contains(filter)) {
                    continue;
                }

                if (ImGui::Selectable(mesh_name.c_str(), mesh_name == this->selected_mesh)) {
                    this->selected_mesh = mesh_name;
                }

                if (ImGui::BeginPopupContextItem(mesh_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
                    bool should_break = false;
                    ImGui::Text("Edit Mesh");

                    if (std::optional name = gui_input_waited("##RenameMeshInput", mesh_name)) {
                        if (!scene->meshes.contains(name.value())) {
                            for (auto& [_, entity] : *scene) {
                                if (entity->mesh_name == mesh_name) {
                                    entity->mesh_name = name.value();
                                }
                            }
                            if (this->selected_mesh == mesh_name) {
                                this->selected_mesh = name.value();
                            }
                            scene->meshes[name.value()] = mesh;
                            scene->meshes.erase(mesh_name);
                            should_break = true;
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                        for (auto& [_, entity] : *scene) {
                            if (entity->mesh_name == mesh_name) {
                                entity->mesh_name = "/";
                            }
                        }
                        if (this->selected_mesh == mesh_name) {
                            this->selected_mesh = "/";
                        }
                        scene->meshes.erase(mesh_name);
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

        void update_mesh_camera()
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

        void render_selected_mesh(kl::GPU* gpu, const Mesh* mesh, const kl::Int2 viewport_size)
        {
            if (viewport_size.x <= 0 || viewport_size.y <= 0) {
                return;
            }

            const kl::Int2 texture_size = render_texture->graphics_buffer_size();
            if (texture_size != viewport_size) {
                render_texture->graphics_buffer = gpu->create_target_texture(viewport_size);
                render_texture->create_target_view(nullptr);
                render_texture->create_shader_view(nullptr);
            }

            gpu->bind_target_depth_views({ render_texture->target_view }, nullptr);
            gpu->clear_target_view(render_texture->target_view, camera->background);

            const kl::Int2 old_viewport_size = gpu->viewport_size();
            gpu->set_viewport_size(viewport_size);

            RenderStates* states = &gui_layer->render_layer->states;
            gpu->bind_raster_state(states->raster_states->wireframe);
            gpu->bind_depth_state(states->depth_states->disabled);

            kl::RenderShaders& render_shaders = states->shader_states->unlit_pass;
            gpu->bind_render_shaders(render_shaders);

            camera->update_aspect_ratio(viewport_size);

            struct VSData
            {
                kl::Float4x4 WVP;
            } vs_data = {};
            vs_data.WVP = camera->camera_matrix();

            struct PSData
            {
                kl::Float4 color;
            } ps_data = {};
            ps_data.color = line_color;

            render_shaders.vertex_shader.update_cbuffer(vs_data);
            render_shaders.pixel_shader.update_cbuffer(ps_data);

            gpu->draw(mesh->graphics_buffer);

            gpu->bind_internal_views();
            gpu->set_viewport_size(old_viewport_size);
        }

        void show_mesh_properties(Mesh* mesh)
        {
            if (ImGui::Begin("Mesh Properties") && mesh) {
                ImGui::Text("Info");

                ImGui::Text("Name: ");
                ImGui::SameLine();
                gui_colored_text(selected_mesh, gui_layer->special_color);

                int vertex_count = static_cast<int>(mesh->data_buffer.size());
                ImGui::DragInt("Vertex Count", &vertex_count, 0);
            }
            ImGui::End();
        }
    };
}
