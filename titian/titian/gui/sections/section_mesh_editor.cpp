#include "main.h"


titian::GUISectionMeshEditor::GUISectionMeshEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
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

void titian::GUISectionMeshEditor::render_gui()
{
    kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
    Scene* scene = &editor_layer->game_layer->scene;
    Mesh* mesh = &scene->get_mesh(this->selected_mesh);

    if (ImGui::Begin("Mesh Editor")) {
        const float available_width = ImGui::GetContentRegionAvail().x;
        ImGui::Columns(2, "MeshEditorColumns", false);

        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Meshes")) {
            display_meshes(gpu, scene);
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
            const kl::Int2 viewport_size = { (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y };
            if (was_focused) {
                update_mesh_camera();
            }
            if (mesh) {
                render_selected_mesh(gpu, mesh, viewport_size);
                const kl::dx::ShaderView& shader_view = render_texture->shader_view;
                ImGui::Image(render_texture->shader_view.Get(), { (float)viewport_size.x, (float)viewport_size.y });
            }

            render_gizmos(mesh);
            was_focused = ImGui::IsWindowFocused();
        }
        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        show_mesh_properties(mesh);
    }
    ImGui::End();
}

void titian::GUISectionMeshEditor::display_meshes(kl::GPU* gpu, Scene* scene)
{
    // New mesh
    if (ImGui::BeginPopupContextWindow("NewMesh", ImGuiPopupFlags_MouseButtonMiddle)) {
        ImGui::Text("New Mesh");

        if (std::optional opt_name = gui_input_waited("##CreateMeshInput", {})) {
            const std::string& name = opt_name.value();
            if (!name.empty() && !scene->meshes.contains(name)) {
                kl::Object mesh = new Mesh(gpu, scene->physics(), scene->cooking());
                mesh->reload();
                scene->meshes[name] = mesh;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    // Meshes
    const std::string filter = gui_input_continuous("Search###MeshEditor");
    for (const auto& [mesh_name, mesh] : scene->meshes) {
        if (!filter.empty() && mesh_name.find(filter) == -1) {
            continue;
        }

        if (ImGui::Selectable(mesh_name.c_str(), mesh_name == this->selected_mesh)) {
            this->selected_mesh = mesh_name;
        }

        if (ImGui::BeginPopupContextItem(mesh_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            ImGui::Text("Edit Mesh");

            if (std::optional opt_name = gui_input_waited("##RenameMeshInput", mesh_name)) {
                const std::string& name = opt_name.value();
                if (!name.empty() && !scene->meshes.contains(name)) {
                    for (auto& [_, entity] : *scene) {
                        if (entity->mesh_name == mesh_name) {
                            entity->mesh_name = name;
                        }
                    }
                    if (this->selected_mesh == mesh_name) {
                        this->selected_mesh = name;
                    }
                    scene->meshes[name] = mesh;
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

void titian::GUISectionMeshEditor::update_mesh_camera()
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

void titian::GUISectionMeshEditor::render_selected_mesh(kl::GPU* gpu, const Mesh* mesh, const kl::Int2 viewport_size)
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
    gpu->bind_raster_state(mesh->render_wireframe ? states->raster_states->wireframe : states->raster_states->solid);
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

    gpu->draw(mesh->graphics_buffer, mesh->casted_topology());

    gpu->bind_internal_views();
    gpu->set_viewport_size(old_viewport_size);
}

void titian::GUISectionMeshEditor::render_gizmos(Mesh* mesh)
{
    kl::Window* window = &editor_layer->game_layer->app_layer->window;

    Mesh::Data& mesh_data = mesh->data_buffer;
    if (m_selected_vertex_index < 0 || m_selected_vertex_index >= mesh_data.size()) {
        return;
    }
    kl::Vertex* selected_vertex = &mesh_data[m_selected_vertex_index];

    const float viewport_tab_height = ImGui::GetWindowContentRegionMin().y;
    const kl::Float2 viewport_position = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + viewport_tab_height };
    const kl::Float2 viewport_size = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    kl::Float3 selected_snap = {};
    if (window->keyboard.shift) {
        selected_snap = kl::Float3{ 0.1f };
    }

    const kl::Float4x4 view_matrix = kl::transpose(camera->view_matrix());
    const kl::Float4x4 projection_matrix = kl::transpose(camera->projection_matrix());
    kl::Float4x4 transform_matrix = kl::transpose(kl::Float4x4::translation(selected_vertex->world));

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD,
        transform_matrix.data, nullptr,
        selected_snap);

    if (ImGuizmo::IsUsing()) {
        kl::Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            decomposed_parts[2], decomposed_parts[1], decomposed_parts[0]);

        selected_vertex->world = decomposed_parts[2];
        mesh->reload();
    }
}

void titian::GUISectionMeshEditor::show_mesh_properties(Mesh* mesh)
{
    kl::Window* window = &editor_layer->game_layer->app_layer->window;

    const int current_scroll = window->mouse.scroll();

    if (ImGui::Begin("Mesh Properties") && mesh) {
        ImGui::Text("Info");

        ImGui::Text("Name: ");
        ImGui::SameLine();
        gui_colored_text(selected_mesh, gui_layer->special_color);

        int vertex_count = static_cast<int>(mesh->data_buffer.size());
        ImGui::DragInt("Vertex Count", &vertex_count, 0);

        std::string topology_name = "Point";
        if (mesh->topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST) {
            topology_name = "Line";
        }
        else if (mesh->topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
            topology_name = "Triangle";
        }

        if (ImGui::BeginCombo("Topology", topology_name.c_str())) {
            if (ImGui::Selectable("Point", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_POINTLIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            }
            if (ImGui::Selectable("Line", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            }
            if (ImGui::Selectable("Triangle", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Render Wireframe", &mesh->render_wireframe);

        ImGui::Separator();
        ImGui::Text("Vertices");

        if (ImGui::Button("Reload")) {
            mesh->reload();
        }

        if (vertex_count > 0) {
            const std::pair window_rect = gui_window_rect();
            if (ImGui::IsMouseHoveringRect(window_rect.first, window_rect.second)) {
                m_starting_vertex_index += m_last_scroll - current_scroll;
            }
            m_starting_vertex_index = std::clamp(m_starting_vertex_index, 0, vertex_count - 1);

            int change_counter = 0;
            for (int i = m_starting_vertex_index; i < (m_starting_vertex_index + m_vertex_display_count) && i < vertex_count; i++) {
                kl::Vertex& vertex = mesh->data_buffer[i];

                const std::string vertex_name = kl::format(i, ". Vertex");
                if (ImGui::Selectable(vertex_name.c_str(), m_selected_vertex_index == i)) {
                    m_selected_vertex_index = (m_selected_vertex_index != i) ? i : -1;
                }

                if (ImGui::BeginPopupContextItem(vertex_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
                    if (ImGui::Button("Delete")) {
                        mesh->data_buffer.erase(mesh->data_buffer.begin() + i);
                        mesh->reload();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                const bool world_edited = ImGui::DragFloat3(kl::format("World##MeshEditor", i).c_str(), vertex.world, 0.1f);
                const bool texture_edited = ImGui::DragFloat2(kl::format("Texture##MeshEditor", i).c_str(), vertex.texture, 0.1f);
                const bool normal_edited = ImGui::DragFloat3(kl::format("Normal##MeshEditor", i).c_str(), vertex.normal, 0.1f);

                if (normal_edited) {
                    vertex.normal = kl::normalize(vertex.normal);
                }

                change_counter += static_cast<int>(world_edited);
                change_counter += static_cast<int>(texture_edited);
                change_counter += static_cast<int>(normal_edited);
            }
            if (change_counter > 0) {
                mesh->reload();
            }
        }

        if (ImGui::BeginPopupContextWindow("EditDisplay", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            ImGui::SliderInt("Display Count", &m_vertex_display_count, 1, 25);
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupContextWindow("NewVertex", ImGuiPopupFlags_MouseButtonMiddle)) {
            ImGui::Text("New Vertex");
            ImGui::SliderInt("Index", &m_new_vertex_index, 0, vertex_count);
            if (ImGui::Button("Create New")) {
                if (m_new_vertex_index >= 0 && m_new_vertex_index <= vertex_count /* This works dw */) {
                    mesh->data_buffer.insert(mesh->data_buffer.begin() + m_new_vertex_index, kl::Vertex());
                    mesh->reload();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    m_last_scroll = current_scroll;
}
