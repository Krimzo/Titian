export module gui_scene_editor;

export import gui_render;
export import deserializer;

kl::Int2 window_mouse_position();
int find_entity_index(const EditorState* state, const kl::Int2& pixel_coords);

void handle_gizmo_operation_change(EditorState* state, int gizmo_operation, ImGuiKey switch_key);
void render_gizmos(EditorState* state);

void gui_scene_editor(EditorState* state)
{
    
}

kl::Int2 window_mouse_position()
{
    const float tab_size = ImGui::GetWindowContentRegionMin().y;
    const ImVec2 window_position = ImGui::GetWindowPos();
    const ImVec2 mouse_position = ImGui::GetMousePos();
    return {
        (int) (mouse_position.x - window_position.x - 0.0f),
        (int) (mouse_position.y - window_position.y - tab_size),
    };
}

int find_entity_index(const EditorState* state, const kl::Int2& pixel_coords)
{
    if (pixel_coords.x < 0 || pixel_coords.y < 0) { return 0; }

    D3D11_BOX source_box = {};
    source_box.left = pixel_coords.x;
    source_box.right = source_box.left + 1;
    source_box.top = pixel_coords.y;
    source_box.bottom = source_box.top + 1;
    source_box.front = 0;
    source_box.back = 1;

    state->gpu->context()->CopySubresourceRegion(state->render_state->picking_staging_texture.Get(), 0, 0, 0, 0, state->render_state->picking_texture.Get(), 0, &source_box);

    float result = 0.0f;
    state->gpu->read_from_resource(&result, state->render_state->picking_staging_texture, sizeof(float));
    return (int) result;
}

void handle_gizmo_operation_change(EditorState* state, const int gizmo_operation, const ImGuiKey switch_key)
{
    static std::map<ImGuiKey, bool> last_key_states = {};

    if (ImGui::IsKeyDown(switch_key)) {
        if (!last_key_states[switch_key]) {
            state->gui_state->gizmo_operation = (state->gui_state->gizmo_operation != gizmo_operation) ? gizmo_operation : 0;
        }
        last_key_states[switch_key] = true;
    }
    else {
        last_key_states[switch_key] = false;
    }
}

void render_gizmos(EditorState* state)
{
    if (!state->gui_state->gizmo_operation) { return; }

    const float viewport_tab_height = ImGui::GetWindowContentRegionMin().y;
    const kl::Float2 viewport_position = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + viewport_tab_height };
    const kl::Float2 viewport_size = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    kl::Float3 selected_snap = {};
    if (state->window->keyboard.shift) {
        static const kl::Float3 predefined_snaps[3] = {
            kl::Float3(0.1f),
            kl::Float3(30.0f),
            kl::Float3(1.0f)
        };

        switch (state->gui_state->gizmo_operation) {
        case ImGuizmo::SCALE:
            selected_snap = predefined_snaps[0];
            break;
        case ImGuizmo::ROTATE:
            selected_snap = predefined_snaps[1];
            break;
        case ImGuizmo::TRANSLATE:
            selected_snap = predefined_snaps[2];
        }
    }

    const kl::Float4x4 view_matrix = kl::transpose(state->scene->camera->view_matrix());
    const kl::Float4x4 projection_matrix = kl::transpose(state->scene->camera->projection_matrix());
    kl::Float4x4 transform_matrix = kl::transpose(state->scene->selected_entity->matrix());

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        (ImGuizmo::OPERATION) state->gui_state->gizmo_operation, (ImGuizmo::MODE) state->gui_state->gizmo_mode,
        transform_matrix.data, nullptr,
        selected_snap);

    if (ImGuizmo::IsUsing()) {
        kl::Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            decomposed_parts[2], decomposed_parts[1], decomposed_parts[0]);

        state->scene->selected_entity->render_scale = decomposed_parts[0];
        state->scene->selected_entity->set_rotation(decomposed_parts[1]);
        state->scene->selected_entity->set_position(decomposed_parts[2]);
    }
}
