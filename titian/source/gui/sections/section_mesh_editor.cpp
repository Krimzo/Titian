#include "titian.h"


titian::GUISectionMeshEditor::GUISectionMeshEditor()
    : GUISection("GUISectionMeshEditor")
{
    sun_direction = kl::normalize(Float3(-0.5f, -0.75f, 1.0f));
    camera.background = RGB{ 30, 30, 30 };
    camera.set_position({ -0.34f, 0.18f, -0.94f });
    camera.speed = 3.1f;
}

void titian::GUISectionMeshEditor::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    Scene& scene = Layers::get<GameLayer>().scene();

    Ref<Mesh> mesh;
    if (scene.meshes.contains(selected_mesh)) {
		mesh = scene.meshes.at(selected_mesh);
	}

    if (im::Begin("Mesh Editor")) {
        const float available_width = im::GetContentRegionAvail().x;
        im::Columns(2, "MeshEditorColumns", false);

        im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
        if (im::BeginChild("Meshes")) {
            display_meshes(scene);
        }
        im::EndChild();
        im::NextColumn();

        im::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        im::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        im::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (im::BeginChild("Mesh View", {}, was_focused)) {
            const Int2 viewport_size = { (int)im::GetContentRegionAvail().x, (int)im::GetContentRegionAvail().y };
            if (was_focused) {
                update_mesh_camera(scene);
            }
            if (mesh) {
                render_selected_mesh(*mesh, viewport_size);
                const dx::ShaderView& shader_view = render_texture.shader_view;
                im::Image(render_texture.shader_view.get(), { (float)viewport_size.x, (float)viewport_size.y });
                render_gizmos(*mesh);
            }
            was_focused = im::IsWindowFocused();
        }
        im::EndChild();

        if (auto file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::MESH) {
                if (auto data = scene.get_assimp_data(file.value())) {
                    const aiScene& ai_scene = *data.value().importer->GetScene();
                    for (uint32_t i = 0; i < ai_scene.mNumMeshes; i++) {
                        Ref mesh = scene.load_assimp_mesh(ai_scene, *ai_scene.mMeshes[i]);
                        scene.meshes[data.value().meshes[i]] = mesh;
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

void titian::GUISectionMeshEditor::display_meshes(Scene& scene)
{
    if (im::BeginPopupContextWindow("NewMesh", ImGuiPopupFlags_MouseButtonMiddle)) {
        im::Text("New Mesh");
        const String name = gui_input_continuous("##CreateMeshInput");
        if (!name.empty() && !scene.meshes.contains(name)) {
            if (im::MenuItem("Basic Mesh")) {
                scene.meshes[name] = new Mesh();
                im::CloseCurrentPopup();
            }
            if (im::MenuItem("Screen Mesh")) {
                Ref mesh = new Mesh();
                mesh->load_triangles(kl::GPU::generate_screen_mesh());
                scene.meshes[name] = mesh;
                im::CloseCurrentPopup();
            }
            if (im::BeginMenu("Plane Mesh")) {
                im::DragFloat("Size", &m_plane_size);
                im::DragInt("Complexity", &m_plane_complexity, 1.0f, 2, 1'000'000);
                if (im::MenuItem("Generate")) {
                    Ref mesh = new Mesh();
                    mesh->load_triangles(kl::GPU::generate_plane_mesh(m_plane_size, m_plane_complexity));
                    scene.meshes[name] = mesh;
                    im::CloseCurrentPopup();
                }
                im::EndMenu();
            }
            if (im::BeginMenu("Cube Mesh")) {
                im::DragFloat("Size", &m_cube_size, 1.0f, 0.0f, 1e6f);
                if (im::MenuItem("Generate")) {
                    Ref mesh = new Mesh();
                    mesh->load_triangles(kl::GPU::generate_cube_mesh(m_cube_size));
                    scene.meshes[name] = mesh;
                    im::CloseCurrentPopup();
                }
                im::EndMenu();
            }
            if (im::BeginMenu("Sphere Mesh")) {
                im::DragFloat("Radius", &m_sphere_radius, 1.0f, 0.0f, 1e6f);
                im::DragInt("Complexity", &m_sphere_complexity, 1.0f, 0, 1'000'000);
                im::Checkbox("Smooth", &m_sphere_smooth);
                if (im::MenuItem("Generate")) {
                    Ref mesh = new Mesh();
                    mesh->load_triangles(kl::GPU::generate_sphere_mesh(m_sphere_radius, m_sphere_complexity, m_sphere_smooth));
                    scene.meshes[name] = mesh;
                    im::CloseCurrentPopup();
                }
                im::EndMenu();
            }
            if (im::BeginMenu("Capsule Mesh")) {
                im::DragFloat("Radius", &m_capsule_radius, 1.0f, 0.0f, 1e6f);
                im::DragFloat("Height", &m_capsule_height, 1.0f, 0.0f, 1e6f);
                im::DragInt("Sectors", &m_capsule_sectors, 1.0f, 1, 1'000'000);
                im::DragInt("Rings", &m_capsule_rings, 1.0f, 1, 1'000'000);
                if (im::MenuItem("Generate")) {
                    Ref mesh = new Mesh();
                    mesh->load_triangles(kl::GPU::generate_capsule_mesh(m_capsule_radius, m_capsule_height, m_capsule_sectors, m_capsule_rings));
                    scene.meshes[name] = mesh;
                    im::CloseCurrentPopup();
                }
                im::EndMenu();
            }
        }
        im::EndPopup();
    }

    const String filter = gui_input_continuous("Search###MeshEditor");
    for (auto& [mesh_name, mesh] : scene.meshes) {
        if (!filter.empty() && mesh_name.find(filter) == -1) {
            continue;
        }

        if (im::Selectable(mesh_name.data(), mesh_name == selected_mesh)) {
            selected_mesh = mesh_name;
        }

        if (im::BeginPopupContextItem(mesh_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            im::Text("Edit Mesh");

            if (auto opt_name = gui_input_waited("##RenameMeshInput", mesh_name)) {
                const auto& new_name = opt_name.value();
                if (!new_name.empty() && !scene.meshes.contains(new_name)) {
                    if (selected_mesh == mesh_name) {
                        selected_mesh = new_name;
                    }
                    for (auto& [_, animation] : scene.animations) {
                        for (auto& name : animation->meshes) {
                            if (name == mesh_name) {
                                name = new_name;
                            }
                        }
                    }
                    scene.meshes[new_name] = mesh;
                    scene.meshes.erase(mesh_name);
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
                if (selected_mesh == mesh_name) {
                    selected_mesh = "/";
                }
                scene.meshes.erase(mesh_name);
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

void titian::GUISectionMeshEditor::update_mesh_camera(Scene& scene)
{
    if (im::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    const int scroll = Layers::get<AppLayer>().window.mouse.scroll();
    if (im::IsMouseDown(ImGuiMouseButton_Right)) {
        const ImVec2 drag_delta = im::GetMouseDragDelta(ImGuiMouseButton_Right);
        camera_info.x = initial_camera_info.x + drag_delta.x * camera.sensitivity;
        camera_info.y = initial_camera_info.y + drag_delta.y * camera.sensitivity;
        camera_info.y = kl::clamp(camera_info.y, -85.0f, 85.0f);

        camera.set_position({
            kl::sin_d(camera_info.x),
            kl::tan_d(camera_info.y),
            kl::cos_d(camera_info.x),
        });

        camera.speed -= scroll * 0.1f;
        camera.speed = kl::max(camera.speed, 0.1f);
    }

    const float camera_distance = camera.speed;
    camera.set_position(kl::normalize(camera.position()) * camera_distance);
    camera.set_forward(-camera.position());
}

void titian::GUISectionMeshEditor::render_selected_mesh(Mesh& mesh, const Int2 viewport_size)
{
    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
    if (viewport_size.x <= 0 || viewport_size.y <= 0)
        return;

    if (render_texture.resolution() != viewport_size) {
        render_texture.texture = gpu.create_target_texture(viewport_size);
        render_texture.create_target_view(nullptr);
        render_texture.create_shader_view(nullptr);
    }
    if (depth_texture.resolution() != viewport_size) {
        dx::TextureDescriptor descriptor = {};
        descriptor.Width = viewport_size.x;
        descriptor.Height = viewport_size.y;
        descriptor.MipLevels = 1;
        descriptor.ArraySize = 1;
        descriptor.Format = DXGI_FORMAT_D32_FLOAT;
        descriptor.SampleDesc.Count = 1;
        descriptor.Usage = D3D11_USAGE_DEFAULT;
        descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_texture.texture = gpu.create_texture(&descriptor, nullptr);
        depth_texture.create_depth_view(nullptr);
    }
    
    gpu.bind_target_depth_view(render_texture.target_view, depth_texture.depth_view);
    gpu.clear_target_view(render_texture.target_view, camera.background);
    gpu.clear_depth_view(depth_texture.depth_view, 1.0f, 0xFF);

    const Int2 old_viewport_size = gpu.viewport_size();
    gpu.set_viewport_size(viewport_size);

    RenderLayer& render_layer = Layers::get<RenderLayer>();
    gpu.bind_raster_state(mesh.render_wireframe ? render_layer.raster_states.wireframe : render_layer.raster_states.solid);
    gpu.bind_depth_state(render_layer.depth_states.enabled);

    kl::Shaders& shaders = render_layer.shader_states.solid_lit_pass;
    gpu.bind_shaders(shaders);

    camera.update_aspect_ratio(viewport_size);

    struct alignas(16) CB
    {
        Float4x4 W;
        Float4x4 WVP;
        Float4 OBJECT_COLOR;
        Float3 SUN_DIRECTION;
        float IS_SKELETAL;
    } cb = {};

    cb.WVP = camera.camera_matrix();
    cb.OBJECT_COLOR = line_color;
    cb.SUN_DIRECTION = sun_direction;
    shaders.upload(cb);

    gpu.draw(mesh.buffer, mesh.d3d_topology(), sizeof(Vertex));

    gpu.bind_internal_views();
    gpu.set_viewport_size(old_viewport_size);
}

void titian::GUISectionMeshEditor::show_mesh_properties(Mesh* mesh)
{
    GUILayer& gui_layer = Layers::get<GUILayer>();
    kl::Window& window = Layers::get<AppLayer>().window;

    if (im::Begin("Mesh Properties") && mesh) {
        im::Text("Mesh Editor");
        if (im::DragFloat3("Sun Direction", &sun_direction.x, 0.01f)) {
            sun_direction = kl::normalize(sun_direction);
        }

        im::Separator();

        im::Text("Info");
        im::Text("Name: ");
        im::SameLine();
        gui_colored_text(selected_mesh, gui_layer.special_color);

        int vertex_count = (int) mesh->vertices.size();
        im::DragInt("Vertex Count", &vertex_count, 0);

        String topology_name;
        if (mesh->topology == MeshTopology::POINTS) {
            topology_name = "Points";
        }
        else if (mesh->topology == MeshTopology::LINES) {
            topology_name = "Lines";
        }
        else {
            topology_name = "Triangles";
        }

        if (im::BeginCombo("Topology", topology_name.data())) {
            if (im::Selectable("Points", mesh->topology == MeshTopology::POINTS)) {
                mesh->topology = MeshTopology::POINTS;
            }
            if (im::Selectable("Lines", mesh->topology == MeshTopology::LINES)) {
                mesh->topology = MeshTopology::LINES;
            }
            if (im::Selectable("Triangles", mesh->topology == MeshTopology::TRIANGLES)) {
                mesh->topology = MeshTopology::TRIANGLES;
            }
            im::EndCombo();
        }

        im::Checkbox("Render Wireframe", &mesh->render_wireframe);

        im::Separator();

        if (vertex_count > 0) {
            const Pair window_rect = gui_window_rect();
            if (im::IsMouseHoveringRect(window_rect.first, window_rect.second)) {
                m_starting_vertex_index -= window.mouse.scroll();
            }
            m_starting_vertex_index = kl::clamp(m_starting_vertex_index, 0, vertex_count - 1);

            int change_counter = 0;
            for (int i = m_starting_vertex_index; i < (m_starting_vertex_index + m_vertex_display_count) && i < vertex_count; i++) {
                Vertex& vertex = mesh->vertices[i];

                const String vertex_name = kl::format(i + 1, ". Vertex");
                if (im::Selectable(vertex_name.data(), m_selected_vertex_index == i)) {
                    m_selected_vertex_index = (m_selected_vertex_index != i) ? i : -1;
                }

                if (im::BeginPopupContextItem(vertex_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
                    if (im::Button("Delete")) {
                        mesh->vertices.erase(mesh->vertices.begin() + i);
                        mesh->reload();
                        im::CloseCurrentPopup();
                    }
                    im::EndPopup();
                }

                const bool world_edited = im::DragFloat3(kl::format("World##MeshEditor", i).data(), &vertex.position.x, 0.1f);
                const bool normal_edited = im::DragFloat3(kl::format("Normal##MeshEditor", i).data(), &vertex.normal.x, 0.1f);
                const bool uv_edited = im::DragFloat2(kl::format("UV##MeshEditor", i).data(), &vertex.uv.x, 0.1f);

                for (int j = 0; j < MAX_BONE_REFS; j++) {
                    int bone_index = vertex.bone_indices[j];
                    if (im::SliderInt(kl::format("BoneIndex##MeshEditor", i, "_", j).data(), &bone_index, 0, 255)) {
                        vertex.bone_indices[j] = (byte) bone_index;
                    }
                    im::DragFloat(kl::format("BoneWeight##MeshEditor", i, "_", j).data(), &vertex.bone_weights[j], 0.01f, 0.0f, 1.0f);
                }
                if (normal_edited) {
                    vertex.normal = kl::normalize(vertex.normal);
                }

                change_counter += int(world_edited);
                change_counter += int(normal_edited);
                change_counter += int(uv_edited);
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
                if (m_new_vertex_index >= 0 && m_new_vertex_index <= vertex_count) {
                    mesh->vertices.insert(mesh->vertices.begin() + m_new_vertex_index, Vertex());
                    mesh->reload();
                    im::CloseCurrentPopup();
                }
            }
            im::EndPopup();
        }
    }
    im::End();
}

void titian::GUISectionMeshEditor::render_gizmos(Mesh& mesh)
{
    kl::Window& window = Layers::get<AppLayer>().window;

    auto& mesh_data = mesh.vertices;
    if (m_selected_vertex_index < 0 || m_selected_vertex_index >= mesh_data.size()) {
        return;
    }
    Vertex& selected_vertex = mesh_data[m_selected_vertex_index];

    const float viewport_tab_height = im::GetWindowContentRegionMin().y;
    const Float2 viewport_position = { im::GetWindowPos().x, im::GetWindowPos().y + viewport_tab_height };
    const Float2 viewport_size = { im::GetWindowWidth(), im::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    Float3 selected_snap = {};
    if (window.keyboard.shift) {
        selected_snap = Float3{ 0.1f };
    }

    const Float4x4 view_matrix = kl::transpose(camera.view_matrix());
    const Float4x4 projection_matrix = kl::transpose(camera.projection_matrix());
    Float4x4 transform_matrix = kl::transpose(Float4x4::translation(selected_vertex.position));

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD,
        transform_matrix.data, nullptr,
        &selected_snap.x);

    if (ImGuizmo::IsUsing()) {
        Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            &decomposed_parts[2].x, &decomposed_parts[1].x, &decomposed_parts[0].x);

        selected_vertex.position = decomposed_parts[2];
        mesh.reload();
    }
}
