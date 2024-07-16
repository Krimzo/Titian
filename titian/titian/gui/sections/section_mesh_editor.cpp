#include "main.h"


titian::GUISectionMeshEditor::GUISectionMeshEditor(const LayerPackage& package)
    : GUISection("GUISectionMeshEditor", package)
{
    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;

    camera = new Camera(scene->physics(), true);
    render_texture = new Texture(gpu);
    depth_texture = new Texture(gpu);

    sun_direction = kl::normalize(kl::Float3(-0.5f, -0.75f, 1.0f));

    camera->background = kl::Color{ 30, 30, 30 };
    camera->set_position({ -0.34f, 0.18f, -0.94f });
    camera->speed = 3.1f;
}

void titian::GUISectionMeshEditor::render_gui()
{
    const TimeBomb _ = this->time_it();

    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;
    Mesh* mesh = &scene->get_mesh(this->selected_mesh);

    if (imgui::Begin("Mesh Editor")) {
        const float available_width = imgui::GetContentRegionAvail().x;
        imgui::Columns(2, "MeshEditorColumns", false);

        imgui::SetColumnWidth(imgui::GetColumnIndex(), available_width * 0.25f);
        if (imgui::BeginChild("Meshes")) {
            display_meshes(gpu, scene);
        }
        imgui::EndChild();
        imgui::NextColumn();

        imgui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        imgui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        imgui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (imgui::BeginChild("Mesh View", {}, was_focused)) {
            const kl::Int2 viewport_size = { (int)imgui::GetContentRegionAvail().x, (int)imgui::GetContentRegionAvail().y };
            if (was_focused) {
                update_mesh_camera();
            }
            if (mesh) {
                render_selected_mesh(gpu, mesh, viewport_size);
                const kl::dx::ShaderView& shader_view = render_texture->shader_view;
                imgui::Image(render_texture->shader_view.Get(), { (float)viewport_size.x, (float)viewport_size.y });
                render_gizmos(mesh);
            }
            was_focused = imgui::IsWindowFocused();
        }
        imgui::EndChild();

        if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
            if (mesh && classify_file(file.value()) == FileType::MESH) {
                const std::filesystem::path path = file.value();
                mesh->data_buffer = kl::parse_obj_file(path.string());
                mesh->reload();
            }
        }

        imgui::PopStyleColor();
        imgui::PopStyleVar(2);

        show_mesh_properties(mesh);
    }
    imgui::End();
}

void titian::GUISectionMeshEditor::display_meshes(kl::GPU* gpu, Scene* scene)
{
    // New mesh
    if (imgui::BeginPopupContextWindow("NewMesh", ImGuiPopupFlags_MouseButtonMiddle)) {
        imgui::Text("New Mesh");

        if (std::optional opt_name = gui_input_waited("##CreateMeshInput", {})) {
            const std::string& name = opt_name.value();
            if (!name.empty() && !scene->meshes.contains(name)) {
                kl::Object mesh = new Mesh(gpu, scene->physics(), scene->cooking());
                mesh->reload();
                scene->meshes[name] = mesh;
                imgui::CloseCurrentPopup();
            }
        }
        imgui::EndPopup();
    }

    // Meshes
    const std::string filter = gui_input_continuous("Search###MeshEditor");
    for (auto& [mesh_name, mesh] : scene->meshes) {
        if (!filter.empty() && mesh_name.find(filter) == -1) {
            continue;
        }

        if (imgui::Selectable(mesh_name.c_str(), mesh_name == this->selected_mesh)) {
            this->selected_mesh = mesh_name;
        }

        if (imgui::BeginPopupContextItem(mesh_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            imgui::Text("Edit Mesh");

            if (std::optional opt_name = gui_input_waited("##RenameMeshInput", mesh_name)) {
                const std::string& name = opt_name.value();
                if (!name.empty() && !scene->meshes.contains(name)) {
                    if (this->selected_mesh == mesh_name) {
                        this->selected_mesh = name;
                    }
                    scene->meshes[name] = mesh;
                    scene->meshes.erase(mesh_name);
                    should_break = true;
                    imgui::CloseCurrentPopup();
                }
            }

            if (imgui::Button("Reload", { -1.0f, 0.0f })) {
                mesh->reload();
                should_break = true;
                imgui::CloseCurrentPopup();
            }

            if (imgui::Button("Delete", { -1.0f, 0.0f })) {
                if (this->selected_mesh == mesh_name) {
                    this->selected_mesh = "/";
                }
                scene->meshes.erase(mesh_name);
                should_break = true;
                imgui::CloseCurrentPopup();
            }

            imgui::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionMeshEditor::update_mesh_camera()
{
    if (imgui::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    const int scroll = app_layer->window.mouse.scroll();
    if (imgui::IsMouseDown(ImGuiMouseButton_Right)) {
        const ImVec2 drag_delta = imgui::GetMouseDragDelta(ImGuiMouseButton_Right);
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

    gpu->bind_target_depth_view(render_texture->target_view, depth_texture->depth_view);
    gpu->clear_target_view(render_texture->target_view, camera->background);
    gpu->clear_depth_view(depth_texture->depth_view, 1.0f, 0xFF);

    const kl::Int2 old_viewport_size = gpu->viewport_size();
    gpu->set_viewport_size(viewport_size);

    RenderStates* states = &render_layer->states;
    gpu->bind_raster_state(mesh->render_wireframe ? states->raster_states->wireframe : states->raster_states->solid);
    gpu->bind_depth_state(states->depth_states->enabled);

    kl::RenderShaders& render_shaders = states->shader_states->solid_lit_pass;
    gpu->bind_render_shaders(render_shaders);

    camera->update_aspect_ratio(viewport_size);

    struct VS_CB
    {
        kl::Float4x4 W;
        alignas(16) kl::Float4x4 WVP;
    };

    const VS_CB vs_cb{
        .W = {},
		.WVP = camera->camera_matrix(),
    };

    struct PS_CB
    {
        kl::Float4 OBJECT_COLOR;
        alignas(16) kl::Float3 SUN_DIRECTION;
    };

    const PS_CB ps_cb{
        .OBJECT_COLOR = line_color,
        .SUN_DIRECTION = sun_direction,
    };

    render_shaders.vertex_shader.update_cbuffer(vs_cb);
    render_shaders.pixel_shader.update_cbuffer(ps_cb);

    gpu->draw(mesh->graphics_buffer, mesh->casted_topology());

    gpu->bind_internal_views();
    gpu->set_viewport_size(old_viewport_size);
}

void titian::GUISectionMeshEditor::render_gizmos(Mesh* mesh)
{
    kl::Window* window = &app_layer->window;

    Mesh::Data& mesh_data = mesh->data_buffer;
    if (m_selected_vertex_index < 0 || m_selected_vertex_index >= mesh_data.size()) {
        return;
    }
    kl::Vertex<float>* selected_vertex = &mesh_data[m_selected_vertex_index];

    const float viewport_tab_height = imgui::GetWindowContentRegionMin().y;
    const kl::Float2 viewport_position = { imgui::GetWindowPos().x, imgui::GetWindowPos().y + viewport_tab_height };
    const kl::Float2 viewport_size = { imgui::GetWindowWidth(), imgui::GetWindowHeight() };

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
    kl::Window* window = &app_layer->window;

    const int current_scroll = window->mouse.scroll();

    if (imgui::Begin("Mesh Properties") && mesh) {
        /*-*/
        imgui::Text("Mesh Editor");
        if (imgui::DragFloat3("Sun Direction", sun_direction, 0.01f)) {
            sun_direction = kl::normalize(sun_direction);
        }

        /*-*/
        imgui::Text("Info");
        imgui::Text("Name: ");
        imgui::SameLine();
        gui_colored_text(selected_mesh, gui_layer->special_color);

        int vertex_count = static_cast<int>(mesh->data_buffer.size());
        imgui::DragInt("Vertex Count", &vertex_count, 0);

        std::string topology_name = "Point";
        if (mesh->topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST) {
            topology_name = "Line";
        }
        else if (mesh->topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
            topology_name = "Triangle";
        }

        if (imgui::BeginCombo("Topology", topology_name.c_str())) {
            if (imgui::Selectable("Point", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_POINTLIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            }
            if (imgui::Selectable("Line", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            }
            if (imgui::Selectable("Triangle", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            }
            imgui::EndCombo();
        }

        imgui::Checkbox("Render Wireframe", &mesh->render_wireframe);

        imgui::Separator();

        if (vertex_count > 0) {
            const std::pair window_rect = gui_window_rect();
            if (imgui::IsMouseHoveringRect(window_rect.first, window_rect.second)) {
                m_starting_vertex_index += m_last_scroll - current_scroll;
            }
            m_starting_vertex_index = std::clamp(m_starting_vertex_index, 0, vertex_count - 1);

            int change_counter = 0;
            for (int i = m_starting_vertex_index; i < (m_starting_vertex_index + m_vertex_display_count) && i < vertex_count; i++) {
                kl::Vertex<float>& vertex = mesh->data_buffer[i];

                const std::string vertex_name = kl::format(i + 1, ". Vertex");
                if (imgui::Selectable(vertex_name.c_str(), m_selected_vertex_index == i)) {
                    m_selected_vertex_index = (m_selected_vertex_index != i) ? i : -1;
                }

                if (imgui::BeginPopupContextItem(vertex_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
                    if (imgui::Button("Delete")) {
                        mesh->data_buffer.erase(mesh->data_buffer.begin() + i);
                        mesh->reload();
                        imgui::CloseCurrentPopup();
                    }
                    imgui::EndPopup();
                }

                const bool world_edited = imgui::DragFloat3(kl::format("World##MeshEditor", i).c_str(), vertex.world, 0.1f);
                const bool texture_edited = imgui::DragFloat2(kl::format("Texture##MeshEditor", i).c_str(), vertex.texture, 0.1f);
                const bool normal_edited = imgui::DragFloat3(kl::format("Normal##MeshEditor", i).c_str(), vertex.normal, 0.1f);

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

        if (imgui::BeginPopupContextWindow("EditDisplay", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            imgui::SliderInt("Display Count", &m_vertex_display_count, 1, 25);
            imgui::EndPopup();
        }

        if (imgui::BeginPopupContextWindow("NewVertex", ImGuiPopupFlags_MouseButtonMiddle)) {
            imgui::Text("New Vertex");
            imgui::SliderInt("Index", &m_new_vertex_index, 0, vertex_count);
            if (imgui::Button("Create New")) {
                if (m_new_vertex_index >= 0 && m_new_vertex_index <= vertex_count /* This works dw */) {
                    mesh->data_buffer.insert(mesh->data_buffer.begin() + m_new_vertex_index, kl::Vertex());
                    mesh->reload();
                    imgui::CloseCurrentPopup();
                }
            }
            imgui::EndPopup();
        }
    }
    imgui::End();

    m_last_scroll = current_scroll;
}
