#include "main.h"


titian::GUISectionViewport::GUISectionViewport(EditorLayer* editor_layer, RenderLayer* render_layer)
    : GUISection("GUISectionViewport")
    , editor_layer(editor_layer)
    , render_layer(render_layer)
{}

void titian::GUISectionViewport::render_gui()
{
    const TimeBomb _ = this->time_it();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

    kl::GPU* gpu = &render_layer->game_layer->app_layer->gpu;
    kl::Object<Scene>& scene = render_layer->game_layer->scene;

    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        // Pre-display
        const ImVec2 content_region = ImGui::GetContentRegionAvail();
        render_layer->resize({ (int) content_region.x, (int) content_region.y });
        editor_layer->is_viewport_focused = ImGui::IsWindowFocused();

        const ImVec2 win_content_min = ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        editor_layer->is_over_viewport = ImGui::IsMouseHoveringRect(win_content_min, win_content_max);

        // Display rendered texture
        ImGui::Image(render_layer->render_texture->shader_view.Get(), content_region);

        // Scene loading
        if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::SCENE) {
                if (const Serializer serializer = { file.value(), false }) {
                    render_layer->game_layer->reset_scene();
                    scene->deserialize(&serializer, nullptr);
                }
                else {
                    Logger::log("Failed to load scene: ", file.value());
                }
            }
        }

        // Handle entity picking
        const ImVec2 viewport_max = ImGui::GetWindowPos() + ImGui::GetWindowSize();
        if (ImGui::IsWindowFocused() && ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), viewport_max)) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver()) {
                m_rect_selection_first = window_mouse_position();
            }
            if (m_rect_selection_first) {
                const kl::Int2 rect_selection_first = m_rect_selection_first.value();
                if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    const kl::Int2 rect_selection_last = window_mouse_position();
                    kl::Float2 min_coords = kl::min(rect_selection_first, rect_selection_last);
                    kl::Float2 max_coords = kl::max(rect_selection_first, rect_selection_last);
                    min_coords += { win_content_min.x, win_content_min.y };
                    max_coords += { win_content_min.x, win_content_min.y };
                    if (max_coords.x == min_coords.x) {
                        max_coords.x += 1.0f;
                    }
                    if (max_coords.y == min_coords.y) {
                        max_coords.y += 1.0f;
                    }

                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    draw_list->AddRectFilled({ min_coords.x, min_coords.y }, { max_coords.x, max_coords.y }, ImColor(255, 255, 255, 50));
                    draw_list->AddRect({ min_coords.x, min_coords.y }, { max_coords.x, max_coords.y }, ImColor(255, 255, 255, 200));
                }
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                    const kl::Int2 rect_selection_last = window_mouse_position();
                    const std::unordered_set<uint32_t> entity_ids = read_entity_ids(rect_selection_first, rect_selection_last);

                    if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && !ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                        editor_layer->selected_entities.clear();
                    }
                    
                    if (entity_ids.size() == 1) {
                        uint32_t counter = 0;
                        for (const auto& [name, _] : *scene) {
                            counter += 1;
                            if (!entity_ids.contains(counter)) {
                                continue;
                            }

                            if (editor_layer->selected_entities.contains(name)) {
                                editor_layer->selected_entities.erase(name);
                            }
                            else {
                                editor_layer->selected_entities.insert(name);
                            }
                            break;
                        }
                    }
                    else if (entity_ids.size() > 1) {
                        uint32_t counter = 0;
                        for (const auto& [name, _] : *scene) {
                            counter += 1;
                            if (!entity_ids.contains(counter)) {
                                continue;
                            }
                            editor_layer->selected_entities.insert(name);
                        }
                    }
                }
            }
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                m_rect_selection_first = std::nullopt;
            }
        }

        // Handle gizmos
        if (ImGui::IsWindowFocused()) {
            handle_gizmo_operation_change(ImGuizmo::OPERATION::SCALE, ImGuiKey_1);
            handle_gizmo_operation_change(ImGuizmo::OPERATION::ROTATE, ImGuiKey_2);
            handle_gizmo_operation_change(ImGuizmo::OPERATION::TRANSLATE, ImGuiKey_3);
        }
        std::unordered_set<Entity*> entities;
        for (const auto& sel_ent : editor_layer->selected_entities) {
            if (Entity* entity = &scene->get_entity(sel_ent)) {
				entities.insert(entity);
            }
        }
        if (!entities.empty()) {
            render_gizmos(entities);
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

std::unordered_set<uint32_t> titian::GUISectionViewport::read_entity_ids(const kl::Int2& first_coords, const kl::Int2& last_coords)
{
    kl::Int2 min_coords = kl::min(first_coords, last_coords);
	kl::Int2 max_coords = kl::max(first_coords, last_coords);
    if (max_coords.x == min_coords.x) {
        max_coords.x += 1;
    }
    if (max_coords.y == min_coords.y) {
        max_coords.y += 1;
    }
    if (min_coords.x < 0 || min_coords.y < 0) {
        return {};
    }

    const kl::Int2 size = max_coords - min_coords;
    const kl::GPU* gpu = &render_layer->game_layer->app_layer->gpu;
    render_layer->resize_staging(size);

    D3D11_BOX source_box = {};
    source_box.left = min_coords.x;
    source_box.top = min_coords.y;
    source_box.right = max_coords.x;
    source_box.bottom = max_coords.y;
    source_box.front = 0;
    source_box.back = 1;
    gpu->context()->CopySubresourceRegion(render_layer->staging_texture->graphics_buffer.Get(),
        0, 0, 0, 0, render_layer->picking_texture->graphics_buffer.Get(), 0, &source_box);

    kl::dx::TextureDescriptor tex_desc = {};
    render_layer->staging_texture->graphics_buffer->GetDesc(&tex_desc);

    std::unordered_set<uint32_t> results;
    kl::dx::MappedSubresourceDescriptor mapped_subresource{};
    gpu->context()->Map(render_layer->staging_texture->graphics_buffer.Get(), 0, D3D11_MAP_READ, NULL, &mapped_subresource) >> kl::verify_result;
    const BYTE* mapped_data = (const BYTE*) mapped_subresource.pData;
    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            const BYTE* value_addr = mapped_data + x * sizeof(float) + y * mapped_subresource.RowPitch;
            const float value_flt = *reinterpret_cast<const float*>(value_addr);
            results.insert(static_cast<uint32_t>(value_flt));
        }
    }
    gpu->context()->Unmap(render_layer->staging_texture->graphics_buffer.Get(), 0);
    return results;
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

void titian::GUISectionViewport::render_gizmos(const std::unordered_set<Entity*>& entities)
{
    if (editor_layer->gizmo_operation == 0)
        return;

    kl::Window* window = &editor_layer->game_layer->app_layer->window;
    Scene* scene = &editor_layer->game_layer->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera)
        return;

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

    kl::Float3 collective_center;
    kl::Float4x4 transform_matrix;
    if (entities.size() == 1) {
        Entity* entity = *entities.begin();
        transform_matrix = kl::transpose(entity->model_matrix());
    }
    else {
        for (Entity* entity : entities) {
            collective_center += entity->position();
        }
        collective_center /= static_cast<float>(entities.size());
        transform_matrix = kl::transpose(kl::Float4x4::translation(collective_center));
    }

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        (ImGuizmo::OPERATION) editor_layer->gizmo_operation, (ImGuizmo::MODE) editor_layer->gizmo_mode,
        transform_matrix.data, nullptr,
        selected_snap);

    if (ImGuizmo::IsUsing()) {
        kl::Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            decomposed_parts[2], decomposed_parts[1], decomposed_parts[0]);

        if (entities.size() == 1) {
            Entity* entity = *entities.begin();
            entity->scale = decomposed_parts[0];
            entity->set_rotation(decomposed_parts[1]);
            entity->set_position(decomposed_parts[2]);
        }
        else {
            const kl::Float3 position_delta = decomposed_parts[2] - collective_center;
            for (Entity* entity : entities) {
                entity->set_position(entity->position() + position_delta);
            }
        }
    }
}
