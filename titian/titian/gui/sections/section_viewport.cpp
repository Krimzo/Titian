#include "main.h"


titian::GUISectionViewport::GUISectionViewport(EditorLayer* editor_layer, RenderLayer* render_layer)
{
    this->editor_layer = editor_layer;
    this->render_layer = render_layer;
}

void titian::GUISectionViewport::render_gui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

    kl::GPU* gpu = &render_layer->game_layer->app_layer->gpu;
    kl::Object<Scene>& scene = render_layer->game_layer->scene;

    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        // Pre-display
        const ImVec2 content_region = ImGui::GetContentRegionAvail();
        render_layer->resize({ (int)content_region.x, (int)content_region.y });
        editor_layer->is_viewport_focused = ImGui::IsWindowFocused();

        const ImVec2 win_content_min = ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        editor_layer->is_over_viewport = ImGui::IsMouseHoveringRect(win_content_min, win_content_max);

        // Display rendered texture
        ImGui::Image(render_layer->render_texture->shader_view.Get(), content_region);

        // Scene loading
        if (const std::optional scene_file = gui_get_drag_drop<std::string>("SceneFile")) {
            scene = new Scene(gpu);
            if (const Serializer serializer = { scene_file.value(), false }) {
                scene->deserialize(&serializer, nullptr);
            }
        }

        // Handle entity picking
        const ImVec2 viewport_max = ImGui::GetWindowPos() + ImGui::GetWindowSize();
        if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), viewport_max) && !ImGuizmo::IsOver()) {
            const kl::Int2 pixel_coords = window_mouse_position();
            const uint32_t entity_id = read_entity_id(pixel_coords);

            if (entity_id == 0) {
                editor_layer->selected_entity = "/";
            }
            else {
                uint32_t counter = 0;
                for (const auto& [name, _] : *scene) {
                    if (++counter == entity_id) {
                        editor_layer->selected_entity = name;
                        break;
                    }
                }
            }
        }

        // Handle gizmos
        if (ImGui::IsWindowFocused()) {
            handle_gizmo_operation_change(ImGuizmo::OPERATION::SCALE, ImGuiKey_1);
            handle_gizmo_operation_change(ImGuizmo::OPERATION::ROTATE, ImGuiKey_2);
            handle_gizmo_operation_change(ImGuizmo::OPERATION::TRANSLATE, ImGuiKey_3);
        }
        if (Entity* entity = &scene->get_entity(editor_layer->selected_entity)) {
            render_gizmos(entity);
        }
    }
    else {
        editor_layer->is_viewport_focused = false;
    }
    ImGui::End();

    ImGui::PopStyleVar();
}

kl::Int2 titian::GUISectionViewport::window_mouse_position()
{
    const float tab_size = ImGui::GetWindowContentRegionMin().y;
    const ImVec2 window_position = ImGui::GetWindowPos();
    const ImVec2 mouse_position = ImGui::GetMousePos();
    return {
        static_cast<int>(mouse_position.x - window_position.x - 0.0f),
        static_cast<int>(mouse_position.y - window_position.y - tab_size),
    };
}

uint32_t titian::GUISectionViewport::read_entity_id(const kl::Int2& pixel_coords)
{
    if (pixel_coords.x < 0 || pixel_coords.y < 0) { return 0; }
    const kl::GPU* gpu = &render_layer->game_layer->app_layer->gpu;

    D3D11_BOX source_box = {};
    source_box.left = pixel_coords.x;
    source_box.right = source_box.left + 1;
    source_box.top = pixel_coords.y;
    source_box.bottom = source_box.top + 1;
    source_box.front = 0;
    source_box.back = 1;
    gpu->context()->CopySubresourceRegion(render_layer->staging_texture->graphics_buffer.Get(),
        0, 0, 0, 0, render_layer->picking_texture->graphics_buffer.Get(), 0, &source_box);

    float result = 0.0f;
    gpu->read_from_resource(&result, render_layer->staging_texture->graphics_buffer.Get(), sizeof(float));

    return static_cast<uint32_t>(result);
}

void titian::GUISectionViewport::handle_gizmo_operation_change(const int operation, const ImGuiKey switch_key)
{
    if (ImGui::IsKeyDown(switch_key)) {
        if (!m_last_key_states[switch_key]) {
            editor_layer->gizmo_operation = editor_layer->gizmo_operation != operation ? operation : 0;
        }
        m_last_key_states[switch_key] = true;
    }
    else {
        m_last_key_states[switch_key] = false;
    }
}

void titian::GUISectionViewport::render_gizmos(Entity* entity)
{
    if (editor_layer->gizmo_operation == 0) { return; }

    kl::Window* window = &editor_layer->game_layer->app_layer->window;
    Scene* scene = &editor_layer->game_layer->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera) { return; }

    const float viewport_tab_height = ImGui::GetWindowContentRegionMin().y;
    const kl::Float2 viewport_position = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + viewport_tab_height };
    const kl::Float2 viewport_size = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    kl::Float3 selected_snap = {};
    if (window->keyboard.shift) {
        static kl::Float3 predefined_snaps[3] = {
            kl::Float3(0.1f),
            kl::Float3(30.0f),
            kl::Float3(1.0f),
        };

        switch (editor_layer->gizmo_operation) {
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

    const kl::Float4x4 view_matrix = kl::transpose(camera->view_matrix());
    const kl::Float4x4 projection_matrix = kl::transpose(camera->projection_matrix());
    kl::Float4x4 transform_matrix = kl::transpose(entity->model_matrix());

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        (ImGuizmo::OPERATION)editor_layer->gizmo_operation, (ImGuizmo::MODE)editor_layer->gizmo_mode,
        transform_matrix.data, nullptr,
        selected_snap);

    if (ImGuizmo::IsUsing()) {
        kl::Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            decomposed_parts[2], decomposed_parts[1], decomposed_parts[0]);

        entity->scale = decomposed_parts[0];
        entity->set_rotation(decomposed_parts[1]);
        entity->set_position(decomposed_parts[2]);
    }
}