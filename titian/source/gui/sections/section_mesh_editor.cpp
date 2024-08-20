#include "titian.h"


titian::GUISectionMeshEditor::GUISectionMeshEditor()
    : GUISection("GUISectionMeshEditor")
{
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    camera = new Camera(scene->physics(), true);
    render_texture = new Texture(gpu);
    depth_texture = new Texture(gpu);

    sun_direction = kl::normalize(Float3(-0.5f, -0.75f, 1.0f));

    camera->background = Color{ 30, 30, 30 };
    camera->set_position({ -0.34f, 0.18f, -0.94f });
    camera->speed = 3.1f;
}

void titian::GUISectionMeshEditor::render_gui()
{
    const TimeBomb _ = this->time_it();

    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;
    Ref mesh = scene->get_mesh(this->selected_mesh);

    if (im::Begin("Mesh Editor")) {
        const float available_width = im::GetContentRegionAvail().x;
        im::Columns(2, "MeshEditorColumns", false);

        im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
        if (im::BeginChild("Meshes")) {
            display_meshes(gpu, scene);
        }
        im::EndChild();
        im::NextColumn();

        im::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        im::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        im::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (im::BeginChild("Mesh View", {}, was_focused)) {
            const Int2 viewport_size = { (int)im::GetContentRegionAvail().x, (int)im::GetContentRegionAvail().y };
            if (was_focused) {
                update_mesh_camera();
            }
            if (mesh) {
                render_selected_mesh(gpu, &mesh, viewport_size);
                const dx::ShaderView& shader_view = render_texture->shader_view;
                im::Image(render_texture->shader_view.get(), { (float)viewport_size.x, (float)viewport_size.y });
                render_gizmos(&mesh);
            }
            was_focused = im::IsWindowFocused();
        }
        im::EndChild();

        if (const Optional file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::MESH) {
                if (Optional data = scene->get_assimp_data(file.value())) {
                    const aiScene* ai_scene = data.value().importer->GetScene();
                    for (uint32_t i = 0; i < ai_scene->mNumMeshes; i++) {
                        Ref mesh = scene->load_assimp_mesh(ai_scene, ai_scene->mMeshes[i]);
                        scene->meshes[data.value().meshes[i]] = mesh;
                    }
                }
            }
        }

        im::PopStyleColor();
        im::PopStyleVar(2);

        show_mesh_properties(&mesh);
    }
    im::End();
}

void titian::GUISectionMeshEditor::display_meshes(kl::GPU* gpu, Scene* scene)
{
    // New mesh
    if (im::BeginPopupContextWindow("NewMesh", ImGuiPopupFlags_MouseButtonMiddle)) {
        im::Text("New Mesh");

        if (Optional opt_name = gui_input_waited("##CreateMeshInput", {})) {
            const String& name = opt_name.value();
            if (!name.empty() && !scene->meshes.contains(name)) {
                Ref mesh = new Mesh(gpu, scene->physics(), scene->cooking());
                mesh->reload();
                scene->meshes[name] = mesh;
                im::CloseCurrentPopup();
            }
        }
        im::EndPopup();
    }

    // Meshes
    const String filter = gui_input_continuous("Search###MeshEditor");
    for (auto& [mesh_name, mesh] : scene->meshes) {
        if (!filter.empty() && mesh_name.find(filter) == -1) {
            continue;
        }

        if (im::Selectable(mesh_name.c_str(), mesh_name == this->selected_mesh)) {
            this->selected_mesh = mesh_name;
        }

        if (im::BeginPopupContextItem(mesh_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            im::Text("Edit Mesh");

            if (Optional opt_name = gui_input_waited("##RenameMeshInput", mesh_name)) {
                const String& name = opt_name.value();
                if (!name.empty() && !scene->meshes.contains(name)) {
                    if (this->selected_mesh == mesh_name) {
                        this->selected_mesh = name;
                    }
                    scene->meshes[name] = mesh;
                    scene->meshes.erase(mesh_name);
                    should_break = true;
                    im::CloseCurrentPopup();
                }
            }

            if (im::Button("Reload", { -1.0f, 0.0f })) {
                mesh->reload();
                should_break = true;
                im::CloseCurrentPopup();
            }

            if (im::Button("Delete", { -1.0f, 0.0f })) {
                if (this->selected_mesh == mesh_name) {
                    this->selected_mesh = "/";
                }
                scene->meshes.erase(mesh_name);
                should_break = true;
                im::CloseCurrentPopup();
            }

            im::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionMeshEditor::update_mesh_camera()
{
    if (im::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    const int scroll = Layers::get<AppLayer>()->window.mouse.scroll();
    if (im::IsMouseDown(ImGuiMouseButton_Right)) {
        const ImVec2 drag_delta = im::GetMouseDragDelta(ImGuiMouseButton_Right);
        camera_info.x = initial_camera_info.x + drag_delta.x * camera->sensitivity;
        camera_info.y = initial_camera_info.y + drag_delta.y * camera->sensitivity;
        camera_info.y = kl::clamp(camera_info.y, -85.0f, 85.0f);

        camera->set_position({
            kl::sin_d(camera_info.x),
            kl::tan_d(camera_info.y),
            kl::cos_d(camera_info.x),
        });

        camera->speed += (last_scroll - scroll) * 0.1f;
        camera->speed = kl::max(camera->speed, 0.1f);
    }
    last_scroll = scroll;

    const float camera_distance = camera->speed;
    camera->set_position(kl::normalize(camera->position()) * camera_distance);
    camera->set_forward(-camera->position());
}

void titian::GUISectionMeshEditor::render_selected_mesh(kl::GPU* gpu, const Mesh* mesh, const Int2 viewport_size)
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
        dx::TextureDescriptor descriptor = {};
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

    const Int2 old_viewport_size = gpu->viewport_size();
    gpu->set_viewport_size(viewport_size);

    RenderStates* states = &Layers::get<RenderLayer>()->states;
    gpu->bind_raster_state(mesh->render_wireframe ? states->raster_states->wireframe : states->raster_states->solid);
    gpu->bind_depth_state(states->depth_states->enabled);

    kl::RenderShaders& render_shaders = states->shader_states->solid_lit_pass;
    gpu->bind_render_shaders(render_shaders);

    camera->update_aspect_ratio(viewport_size);

    struct alignas(16) VS_CB
    {
        Float4x4 W;
        Float4x4 WVP;
        float IS_SKELETAL{};
    };

    const VS_CB vs_cb{
        .W = {},
        .WVP = camera->camera_matrix(),
        .IS_SKELETAL = 0.0f,
    };

    struct alignas(16) PS_CB
    {
        Float4 OBJECT_COLOR;
        Float3 SUN_DIRECTION;
    };

    const PS_CB ps_cb{
        .OBJECT_COLOR = line_color,
        .SUN_DIRECTION = sun_direction,
    };

    render_shaders.vertex_shader.update_cbuffer(vs_cb);
    render_shaders.pixel_shader.update_cbuffer(ps_cb);

    gpu->draw(mesh->graphics_buffer, mesh->casted_topology(), sizeof(Vertex));

    gpu->bind_internal_views();
    gpu->set_viewport_size(old_viewport_size);
}

void titian::GUISectionMeshEditor::render_gizmos(Mesh* mesh)
{
    kl::Window* window = &Layers::get<AppLayer>()->window;

    Mesh::Data& mesh_data = mesh->data_buffer;
    if (m_selected_vertex_index < 0 || m_selected_vertex_index >= mesh_data.size()) {
        return;
    }
    Vertex* selected_vertex = &mesh_data[m_selected_vertex_index];

    const float viewport_tab_height = im::GetWindowContentRegionMin().y;
    const Float2 viewport_position = { im::GetWindowPos().x, im::GetWindowPos().y + viewport_tab_height };
    const Float2 viewport_size = { im::GetWindowWidth(), im::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    Float3 selected_snap = {};
    if (window->keyboard.shift) {
        selected_snap = Float3{ 0.1f };
    }

    const Float4x4 view_matrix = kl::transpose(camera->view_matrix());
    const Float4x4 projection_matrix = kl::transpose(camera->projection_matrix());
    Float4x4 transform_matrix = kl::transpose(Float4x4::translation(selected_vertex->world));

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD,
        transform_matrix.data, nullptr,
        selected_snap);

    if (ImGuizmo::IsUsing()) {
        Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            decomposed_parts[2], decomposed_parts[1], decomposed_parts[0]);

        selected_vertex->world = decomposed_parts[2];
        mesh->reload();
    }
}

void titian::GUISectionMeshEditor::show_mesh_properties(Mesh* mesh)
{
    GUILayer* gui_layer = Layers::get<GUILayer>();
    kl::Window* window = &Layers::get<AppLayer>()->window;

    const int current_scroll = window->mouse.scroll();

    if (im::Begin("Mesh Properties") && mesh) {
        /*-*/
        im::Text("Mesh Editor");
        if (im::DragFloat3("Sun Direction", sun_direction, 0.01f)) {
            sun_direction = kl::normalize(sun_direction);
        }

        im::Separator();

        /*-*/
        im::Text("Info");
        im::Text("Name: ");
        im::SameLine();
        gui_colored_text(selected_mesh, gui_layer->special_color);

        int vertex_count = static_cast<int>(mesh->data_buffer.size());
        im::DragInt("Vertex Count", &vertex_count, 0);

        String topology_name = "Point";
        if (mesh->topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST) {
            topology_name = "Line";
        }
        else if (mesh->topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
            topology_name = "Triangle";
        }

        if (im::BeginCombo("Topology", topology_name.c_str())) {
            if (im::Selectable("Point", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_POINTLIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            }
            if (im::Selectable("Line", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            }
            if (im::Selectable("Triangle", mesh->topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)) {
                mesh->topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            }
            im::EndCombo();
        }

        im::Checkbox("Render Wireframe", &mesh->render_wireframe);

        im::Separator();

        if (vertex_count > 0) {
            const Pair window_rect = gui_window_rect();
            if (im::IsMouseHoveringRect(window_rect.first, window_rect.second)) {
                m_starting_vertex_index += m_last_scroll - current_scroll;
            }
            m_starting_vertex_index = kl::clamp(m_starting_vertex_index, 0, vertex_count - 1);

            int change_counter = 0;
            for (int i = m_starting_vertex_index; i < (m_starting_vertex_index + m_vertex_display_count) && i < vertex_count; i++) {
                Vertex& vertex = mesh->data_buffer[i];

                const String vertex_name = kl::format(i + 1, ". Vertex");
                if (im::Selectable(vertex_name.c_str(), m_selected_vertex_index == i)) {
                    m_selected_vertex_index = (m_selected_vertex_index != i) ? i : -1;
                }

                if (im::BeginPopupContextItem(vertex_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
                    if (im::Button("Delete")) {
                        mesh->data_buffer.erase(mesh->data_buffer.begin() + i);
                        mesh->reload();
                        im::CloseCurrentPopup();
                    }
                    im::EndPopup();
                }

                const bool world_edited = im::DragFloat3(kl::format("World##MeshEditor", i).c_str(), vertex.world, 0.1f);
                const bool texture_edited = im::DragFloat2(kl::format("Texture##MeshEditor", i).c_str(), vertex.texture, 0.1f);
                const bool normal_edited = im::DragFloat3(kl::format("Normal##MeshEditor", i).c_str(), vertex.normal, 0.1f);

                for (int j = 0; j < MAX_BONE_REFS; j++) {
                    int bone_index = vertex.bone_indices[j];
                    if (im::SliderInt(kl::format("BoneIndex##MeshEditor", i, "_", j).c_str(), &bone_index, 0, 255)) {
                        vertex.bone_indices[j] = (uint8_t) bone_index;
                    }
                    im::DragFloat(kl::format("BoneWeight##MeshEditor", i, "_", j).c_str(), &vertex.bone_weights[j], 0.01f, 0.0f, 1.0f);
                }

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

        if (im::BeginPopupContextWindow("EditDisplay", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            im::SliderInt("Display Count", &m_vertex_display_count, 1, 25);
            im::EndPopup();
        }

        if (im::BeginPopupContextWindow("NewVertex", ImGuiPopupFlags_MouseButtonMiddle)) {
            im::Text("New Vertex");
            im::SliderInt("Index", &m_new_vertex_index, 0, vertex_count);
            if (im::Button("Create New")) {
                if (m_new_vertex_index >= 0 && m_new_vertex_index <= vertex_count /* This works dw */) {
                    mesh->data_buffer.insert(mesh->data_buffer.begin() + m_new_vertex_index, Vertex());
                    mesh->reload();
                    im::CloseCurrentPopup();
                }
            }
            im::EndPopup();
        }
    }
    im::End();

    m_last_scroll = current_scroll;
}
