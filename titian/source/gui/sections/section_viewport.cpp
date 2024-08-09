#include "titian.h"


titian::GUISectionViewport::GUISectionViewport()
    : GUISection("GUISectionViewport")
{}

void titian::GUISectionViewport::render_gui()
{
    const TimeBomb _ = this->time_it();

    im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

    AppLayer* app_layer = Layers::get<AppLayer>();
    RenderLayer* render_layer = Layers::get<RenderLayer>();
	GameLayer* game_layer = Layers::get<GameLayer>();
    EditorLayer* editor_layer = Layers::get<EditorLayer>();

    kl::GPU* gpu = &app_layer->gpu;
    Ref<Scene>& scene = game_layer->scene;

    if (im::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        // Pre-display
        const ImVec2 content_region = im::GetContentRegionAvail();
        render_layer->resize({ (int) content_region.x, (int) content_region.y });
        editor_layer->is_viewport_focused = im::IsWindowFocused();

        const ImVec2 win_content_min = im::GetWindowPos() + im::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        editor_layer->is_over_viewport = im::IsMouseHoveringRect(win_content_min, win_content_max);

        // Display rendered texture
        im::Image(render_layer->screen_texture->shader_view.get(), content_region);

        // Scene loading
        if (const Optional file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::SCENE) {
                if (const BinarySerializer serializer{ file.value(), false }) {
                    game_layer->reset_scene();
                    scene->deserialize(&serializer, nullptr);
                }
                else {
                    Logger::log("Failed to load scene: ", file.value());
                }
            }
        }

        // Handle entity picking
        const ImVec2 viewport_max = im::GetWindowPos() + im::GetWindowSize();
        if (im::IsWindowFocused() && im::IsMouseHoveringRect(im::GetWindowPos(), viewport_max)) {
            if (im::IsKeyPressed(ImGuiKey_Delete)) {
                for (auto& name : editor_layer->selected_entities) {
					scene->remove_entity(name);
                }
                editor_layer->selected_entities.clear();
            }
            if (im::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver()) {
                m_rect_selection_first = window_mouse_position();
            }
            if (m_rect_selection_first) {
                const Int2 rect_selection_first = m_rect_selection_first.value();
                if (im::IsMouseDown(ImGuiMouseButton_Left)) {
                    const Int2 rect_selection_last = window_mouse_position();
                    Float2 min_coords = kl::min(rect_selection_first, rect_selection_last);
                    Float2 max_coords = kl::max(rect_selection_first, rect_selection_last);
                    min_coords += { win_content_min.x, win_content_min.y };
                    max_coords += { win_content_min.x, win_content_min.y };
                    if (max_coords.x == min_coords.x) {
                        max_coords.x += 1.0f;
                    }
                    if (max_coords.y == min_coords.y) {
                        max_coords.y += 1.0f;
                    }

                    ImDrawList* draw_list = im::GetWindowDrawList();
                    draw_list->AddRectFilled({ min_coords.x, min_coords.y }, { max_coords.x, max_coords.y }, ImColor(255, 255, 255, 50));
                    draw_list->AddRect({ min_coords.x, min_coords.y }, { max_coords.x, max_coords.y }, ImColor(255, 255, 255, 200));
                }
                if (im::IsMouseReleased(ImGuiMouseButton_Left)) {
                    const Int2 rect_selection_last = window_mouse_position();
                    const Set<uint32_t> entity_ids = read_entity_ids(rect_selection_first, rect_selection_last);

                    if (!im::IsKeyDown(ImGuiKey_LeftCtrl) && !im::IsKeyDown(ImGuiKey_LeftShift)) {
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
            if (!im::IsMouseDown(ImGuiMouseButton_Left)) {
                m_rect_selection_first = std::nullopt;
            }
        }

        // Handle gizmos
        if (im::IsWindowFocused()) {
            handle_gizmo_operation_change(ImGuizmo::OPERATION::SCALE, ImGuiKey_1);
            handle_gizmo_operation_change(ImGuizmo::OPERATION::ROTATE, ImGuiKey_2);
            handle_gizmo_operation_change(ImGuizmo::OPERATION::TRANSLATE, ImGuiKey_3);
        }
        Set<Entity*> entities;
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
    im::End();

    im::PopStyleVar();
}

titian::Int2 titian::GUISectionViewport::window_mouse_position()
{
    const float tab_size = im::GetWindowContentRegionMin().y;
    const ImVec2 window_position = im::GetWindowPos();
    const ImVec2 mouse_position = im::GetMousePos();
    return {
        static_cast<int>(mouse_position.x - window_position.x - 0.0f),
        static_cast<int>(mouse_position.y - window_position.y - tab_size),
    };
}

titian::Set<uint32_t> titian::GUISectionViewport::read_entity_ids(const Int2& first_coords, const Int2& last_coords)
{
    Int2 min_coords = kl::min(first_coords, last_coords);
	Int2 max_coords = kl::max(first_coords, last_coords);
    if (max_coords.x == min_coords.x) {
        max_coords.x += 1;
    }
    if (max_coords.y == min_coords.y) {
        max_coords.y += 1;
    }
    if (min_coords.x < 0 || min_coords.y < 0) {
        return {};
    }

    RenderLayer* render_layer = Layers::get<RenderLayer>();
    const Int2 size = max_coords - min_coords;
    const kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

    render_layer->resize_staging(size);
    gpu->copy_resource_region(render_layer->editor_staging_texture->graphics_buffer, render_layer->editor_picking_texture->graphics_buffer, min_coords, max_coords);

    Vector<float> values(size.x * size.y);
    gpu->read_from_texture(values.data(), render_layer->editor_staging_texture->graphics_buffer, size, sizeof(float));

    Set<uint32_t> results;
    for (float value : values) {
        results.insert(static_cast<uint32_t>(value));
    }
    return results;
}

void titian::GUISectionViewport::handle_gizmo_operation_change(const int operation, const ImGuiKey switch_key)
{
    EditorLayer* editor_layer = Layers::get<EditorLayer>();
    if (im::IsKeyDown(switch_key)) {
        if (!m_last_key_states[switch_key]) {
            editor_layer->gizmo_operation = editor_layer->gizmo_operation != operation ? operation : 0;
        }
        m_last_key_states[switch_key] = true;
    }
    else {
        m_last_key_states[switch_key] = false;
    }
}

void titian::GUISectionViewport::render_gizmos(const Set<Entity*>& entities)
{
	EditorLayer* editor_layer = Layers::get<EditorLayer>();
	GameLayer* game_layer = Layers::get<GameLayer>();
	AppLayer* app_layer = Layers::get<AppLayer>();

    if (editor_layer->gizmo_operation == 0)
        return;

    kl::Window* window = &app_layer->window;
    Scene* scene = &game_layer->scene;

    Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!camera)
        return;

    const float viewport_tab_height = im::GetWindowContentRegionMin().y;
    const Float2 viewport_position = { im::GetWindowPos().x, im::GetWindowPos().y + viewport_tab_height };
    const Float2 viewport_size = { im::GetWindowWidth(), im::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    Float3 selected_snap = {};
    if (window->keyboard.shift) {
        static Float3 predefined_snaps[3] = {
            Float3(0.1f),
            Float3(30.0f),
            Float3(1.0f),
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

    const Float4x4 view_matrix = kl::transpose(camera->view_matrix());
    const Float4x4 projection_matrix = kl::transpose(camera->projection_matrix());

    Float3 collective_center;
    Float4x4 transform_matrix;
    if (entities.size() == 1) {
        Entity* entity = *entities.begin();
        transform_matrix = kl::transpose(entity->model_matrix());
    }
    else {
        for (Entity* entity : entities) {
            collective_center += entity->position();
        }
        collective_center /= static_cast<float>(entities.size());
        transform_matrix = kl::transpose(Float4x4::translation(collective_center));
    }

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        (ImGuizmo::OPERATION) editor_layer->gizmo_operation, (ImGuizmo::MODE) editor_layer->gizmo_mode,
        transform_matrix.data, nullptr,
        selected_snap);

    if (ImGuizmo::IsUsing()) {
        Float3 decomposed_parts[3] = {};
        ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
            decomposed_parts[2], decomposed_parts[1], decomposed_parts[0]);

        if (entities.size() == 1) {
            Entity* entity = *entities.begin();
            entity->scale = decomposed_parts[0];
            entity->set_rotation(decomposed_parts[1]);
            entity->set_position(decomposed_parts[2]);
        }
        else {
            const Float3 position_delta = decomposed_parts[2] - collective_center;
            for (Entity* entity : entities) {
                entity->set_position(entity->position() + position_delta);
            }
        }
    }
}
