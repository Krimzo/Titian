export module gui_mesh_editor;

export import gui_render;

void display_meshes(EditorState* state);
std::string find_mesh_name(EditorState* state, const kl::Object<kl::Mesh>& mesh);
void update_mesh_camera(EditorState* state);
void render_selected_mesh(EditorState* state, const kl::Object<kl::Mesh>& mesh, const kl::Int2& viewport_size);

export void gui_mesh_editor(EditorState* state)
{
    if (ImGui::Begin("Mesh Editor")) {
        const float available_width = ImGui::GetContentRegionAvail().x;
        ImGui::Columns(2, "MeshEditorColumns", false);

        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Meshes")) {
            display_meshes(state);
        }
        ImGui::EndChild();

        const std::optional mesh_file_path = gui::drag_drop::read_data<std::string>("MeshFile");
        if (mesh_file_path) {
            const std::filesystem::path path = mesh_file_path.value();
            const std::string mesh_name = path.filename().string();
            if (!state->scene->meshes.contains(mesh_name)) {
                const kl::MeshData mesh_data = kl::parse_obj_file(path.string());
                const kl::Object new_mesh = new kl::Mesh(&state->gpu, &state->scene, mesh_data);
                state->scene->meshes[mesh_name] = new_mesh;
            }
        }
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4&) state->gui_state->color_special);
        static bool was_focused = true;

        const kl::Object mesh = state->gui_state->mesh_editor.selected_mesh;
        if (ImGui::BeginChild("Mesh View", {}, was_focused)) {
            const kl::Int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
            if (was_focused) update_mesh_camera(state);
            if (mesh) {
                render_selected_mesh(state, mesh, viewport_size);
                const kl::dx::ShaderView& shader_view = state->gui_state->mesh_editor.render_texture->shader_view;
                if (shader_view) ImGui::Image(shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
            }
            was_focused = ImGui::IsWindowFocused();
        }
        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
        
        if (ImGui::Begin("Mesh Info") && mesh) {
            const std::string mesh_name = find_mesh_name(state, mesh);
            int vertex_count = (int) mesh->data_buffer.size();

            ImGui::BulletText(mesh_name.c_str());
            ImGui::DragInt("Vertex Count", &vertex_count, 0);
        }
        ImGui::End();
    }
    ImGui::End();
}

void display_meshes(EditorState* state)
{
    kl::Object<kl::Mesh>& selected_mesh = state->gui_state->mesh_editor.selected_mesh;

    for (const auto& [mesh_name, mesh] : state->scene->meshes) {
        if (ImGui::Selectable(mesh_name.c_str(), mesh == selected_mesh)) {
            selected_mesh = mesh;
        }

        if (ImGui::BeginPopupContextItem(mesh_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            ImGui::Text("Edit Mesh");

            char name_input[31] = {};
            memcpy(name_input, mesh_name.c_str(), std::min(mesh_name.size(), std::size(name_input) - 1));

            if (ImGui::InputText("##RenameMeshInput", name_input, std::size(name_input) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (!state->scene->meshes.contains(name_input)) {
                    state->scene->meshes[name_input] = mesh;
                    state->scene->meshes.erase(mesh_name);
                    ImGui::CloseCurrentPopup();
                    should_break = true;
                }
            }
            
            if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                if (mesh == selected_mesh) {
                    selected_mesh = nullptr;
                }
                for (auto& [_, entity] : *state->scene) {
                    if (entity->mesh == mesh) {
                        entity->mesh = nullptr;
                    }
                }
                state->scene->meshes.erase(mesh_name);
                ImGui::CloseCurrentPopup();
                should_break = true;
            }

            ImGui::EndPopup();
            if (should_break) break;
        }
    }
}

std::string find_mesh_name(EditorState* state, const kl::Object<kl::Mesh>& mesh)
{
    for (const auto& [mesh_name, scene_mesh] : state->scene->meshes) {
        if (scene_mesh == mesh) {
            return mesh_name;
        }
    }
    return { "unknown" };
}

void update_mesh_camera(EditorState* state)
{
    static kl::Float2 initial_camera_info = { 40, 30 };
    static kl::Float2 camera_info = initial_camera_info;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    kl::Camera& camera = state->gui_state->mesh_editor.camera;
    
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        const ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        camera_info.x = initial_camera_info.x + drag_delta.x * camera.sensitivity;
        camera_info.y = initial_camera_info.y + drag_delta.y * camera.sensitivity;
        camera_info.y = kl::clamp(camera_info.y, -85.0f, 85.0f);

        camera.origin.x = sin(camera_info.x * kl::TO_RADIANS);
        camera.origin.z = cos(camera_info.x * kl::TO_RADIANS);
        camera.origin.y = tan(camera_info.y * kl::TO_RADIANS);
    }

    static int last_scroll = 0;
    const int scroll = state->window->mouse.scroll();
    camera.speed += (last_scroll - scroll) * 0.1f;
    camera.speed = std::max(camera.speed, 0.1f);
    last_scroll = scroll;

    const float camera_distance = camera.speed;
    camera.origin = kl::normalize(camera.origin) * camera_distance;
    camera.set_forward(-camera.origin);
}

void render_selected_mesh(EditorState* state, const kl::Object<kl::Mesh>& mesh, const kl::Int2& viewport_size)
{
    if (!state->gui_state->mesh_editor.render_texture) {
        state->gui_state->mesh_editor.render_texture = new kl::Texture(&state->gpu);
    }

    kl::dx::Texture& render_texture = state->gui_state->mesh_editor.render_texture->graphics_buffer;
    kl::dx::TargetView& target_view = state->gui_state->mesh_editor.render_texture->target_view;
    kl::dx::ShaderView& shader_view = state->gui_state->mesh_editor.render_texture->shader_view;

    if (viewport_size.x <= 0 || viewport_size.y <= 0) {
        return;
    }

    kl::Int2 texture_size = {};
    if (render_texture) {
        kl::dx::TextureDescriptor descriptor = {};
        render_texture->GetDesc(&descriptor);
        texture_size.x = (int) descriptor.Width;
        texture_size.y = (int) descriptor.Height;
    }

    if (!target_view || texture_size != viewport_size) {
        render_texture = state->gpu->create_target_texture(viewport_size);
        if (!render_texture) {
            return;
        }
        state->gui_state->mesh_editor.render_texture->create_target_view();
        state->gui_state->mesh_editor.render_texture->create_shader_view();
    }

    state->gpu->bind_target_depth_views({ target_view }, nullptr);
    state->gpu->clear_target_view(target_view, kl::Color { 30, 30, 30 });
    state->gpu->set_viewport_size(viewport_size);

    state->gpu->bind_raster_state(state->raster_states.wireframe);
    state->gpu->bind_depth_state(state->depth_states.disabled);

    kl::RenderShaders& shaders = state->gui_state->mesh_editor.shaders;
    if (!shaders) {
        shaders = state->render_shaders.object_single;
    }
    state->gpu->bind_render_shaders(shaders);

    kl::Camera& camera = state->gui_state->mesh_editor.camera;
    camera.update_aspect_ratio(viewport_size);

    class MeshEditorVS
    {
    public:
        kl::Float4x4 WVP;
    } vs_data = {};

    vs_data.WVP = camera.matrix();
    shaders.vertex_shader.update_cbuffer(vs_data);

    class MeshEditorPS
    {
    public:
        kl::Float4 color;
    } ps_data = {};

    ps_data.color = kl::colors::WHITE;
    shaders.pixel_shader.update_cbuffer(ps_data);

    state->gpu->draw(mesh->graphics_buffer);
    state->gpu->bind_internal_views();
}
