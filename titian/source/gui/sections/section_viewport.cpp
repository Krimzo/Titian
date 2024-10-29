#include "titian.h"


titian::GUISectionViewport::GUISectionViewport()
    : GUISection("GUISectionViewport")
{}

void titian::GUISectionViewport::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

    AppLayer& app_layer = AppLayer::get();
    RenderLayer& render_layer = RenderLayer::get();
	GameLayer& game_layer = GameLayer::get();
    EditorLayer& editor_layer = EditorLayer::get();

    kl::GPU& gpu = app_layer.gpu;
    Scene& scene = game_layer.scene();
    Camera* main_camera = scene.get_casted<Camera>(scene.main_camera_name);

    if (im::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        const ImVec2 content_region = im::GetContentRegionAvail();
        editor_layer.is_viewport_focused = im::IsWindowFocused();

        const ImVec2 win_content_min = im::GetWindowPos() + im::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        editor_layer.is_over_viewport = im::IsMouseHoveringRect(win_content_min, win_content_max);

        if (main_camera) {
            main_camera->resize({ (int) content_region.x, (int) content_region.y });
            im::Image(main_camera->screen_texture.shader_view.get(), content_region);
        }
        else {
            im::Image(nullptr, content_region);
        }

        const auto get_mouse_3d = [&]
        {
            if (!main_camera) {
                return Float3{};
            }
            if (auto pos = read_depth_texture(window_mouse_position())) {
                return pos.value();
            }
            const kl::Ray pixel_ray{
                main_camera->position(),
                kl::inverse(main_camera->camera_matrix()),
                window_to_ndc(window_mouse_position()),
            };
            return pixel_ray.origin + pixel_ray.direction() * 10.0f;
        };

        if (auto file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::BINARY_SCENE) {
                if (const BinarySerializer serializer{ file.value(), false }) {
                    game_layer.reset_scene();
                    scene.deserialize(serializer);
                }
                else {
                    Logger::log("Failed to load BINARY scene: ", file.value());
                }
            }
            else if (classify_file(file.value()) == FileType::TEXT_SCENE) {
                if (const TextSerializer serializer{ file.value(), false }) {
                    game_layer.reset_scene();
                    scene.deserialize(serializer);
                }
                else {
                    Logger::log("Failed to load TEXT scene: ", file.value());
                }
            }
        }
        if (auto entity_type = gui_get_drag_drop<String>(DRAG_BASIC_ENTITY_ID)) {
            const String name = Scene::generate_unique_name(entity_type.value(), scene.entities());
            const Float3 position = get_mouse_3d();
            if (entity_type.value() == DRAG_ENTITY_ENTITY) {
                Ref entity = new Entity();
                entity->set_position(position);
				scene.add_entity(name, entity);
            }
            else if (entity_type.value() == DRAG_ENTITY_CAMERA) {
                Ref entity = new Camera();
                entity->set_position(position);
                scene.add_entity(name, entity);
            }
            else if (entity_type.value() == DRAG_ENTITY_AMBIENT) {
                Ref entity = new AmbientLight();
                entity->set_position(position);
                scene.add_entity(name, entity);
            }
            else if (entity_type.value() == DRAG_ENTITY_DIRECTIONAL) {
                Ref entity = new DirectionalLight();
                entity->set_position(position);
                scene.add_entity(name, entity);
            }
        }
        if (auto entity_type = gui_get_drag_drop<String>(DRAG_ANIMATION_ENTITY_ID)) {
            const String name = Scene::generate_unique_name(entity_type.value(), scene.entities());
            const Float3 position = get_mouse_3d();
            Ref entity = new Entity();
			entity->set_position(position);
            entity->animation_name = entity_type.value();
            entity->material_name = "white";
            scene.add_entity(name, entity);
        }

        const ImVec2 viewport_max = im::GetWindowPos() + im::GetWindowSize();
        if (im::IsWindowFocused() && im::IsMouseHoveringRect(im::GetWindowPos(), viewport_max)) {
            if (im::IsKeyPressed(ImGuiKey_Delete)) {
                for (auto& name : editor_layer.selected_entities) {
					scene.remove_entity(name);
                }
                editor_layer.selected_entities.clear();
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
                    const Set<uint32_t> entity_ids = read_id_texture(rect_selection_first, rect_selection_last);

                    if (!im::IsKeyDown(ImGuiKey_LeftCtrl) && !im::IsKeyDown(ImGuiKey_LeftShift)) {
                        editor_layer.selected_entities.clear();
                    }
                    
                    if (entity_ids.size() == 1) {
                        uint32_t counter = 0;
                        for (const auto& [name, _] : scene.entities()) {
                            counter += 1;
                            if (!entity_ids.contains(counter)) {
                                continue;
                            }

                            if (editor_layer.selected_entities.contains(name)) {
                                editor_layer.selected_entities.erase(name);
                            }
                            else {
                                editor_layer.selected_entities.insert(name);
                            }
                            break;
                        }
                    }
                    else if (entity_ids.size() > 1) {
                        uint32_t counter = 0;
                        for (const auto& [name, _] : scene.entities()) {
                            counter += 1;
                            if (!entity_ids.contains(counter)) {
                                continue;
                            }
                            editor_layer.selected_entities.insert(name);
                        }
                    }
                }
            }
            if (!im::IsMouseDown(ImGuiMouseButton_Left)) {
                m_rect_selection_first = std::nullopt;
            }
        }

        if (im::IsWindowFocused()) {
            if (im::IsKeyPressed(ImGuiKey_1)) {
                editor_layer.gizmo_operation = (editor_layer.gizmo_operation != ImGuizmo::OPERATION::SCALE)
                    ? ImGuizmo::OPERATION::SCALE
                    : ImGuizmo::OPERATION::NONE;
            }
            if (im::IsKeyPressed(ImGuiKey_2)) {
                editor_layer.gizmo_operation = (editor_layer.gizmo_operation != ImGuizmo::OPERATION::ROTATE)
                    ? ImGuizmo::OPERATION::ROTATE
                    : ImGuizmo::OPERATION::NONE;
            }
            if (im::IsKeyPressed(ImGuiKey_3)) {
                editor_layer.gizmo_operation = (editor_layer.gizmo_operation != ImGuizmo::OPERATION::TRANSLATE)
                    ? ImGuizmo::OPERATION::TRANSLATE
                    : ImGuizmo::OPERATION::NONE;
            }
            if (im::IsKeyPressed(ImGuiKey_4)) {
                editor_layer.gizmo_mode = (editor_layer.gizmo_mode != ImGuizmo::MODE::WORLD)
                    ? ImGuizmo::MODE::WORLD
                    : ImGuizmo::MODE::LOCAL;
            }
        }
        Set<Entity*> entities;
        for (const auto& sel_ent : editor_layer.selected_entities) {
            if (Entity* entity = scene.helper_get_entity(sel_ent)) {
				entities.insert(entity);
            }
        }
        if (!entities.empty()) {
            render_gizmos(entities);
        }
    }
    else {
        editor_layer.is_viewport_focused = false;
    }
    im::End();

    im::PopStyleVar();
}

titian::Int2 titian::GUISectionViewport::window_mouse_position() const
{
    const ImVec2 window_position = im::GetWindowPos();
    const ImVec2 mouse_position = im::GetMousePos();
    const float tab_size = im::GetWindowContentRegionMin().y;
    return {
        int(mouse_position.x - window_position.x - 0.0f),
        int(mouse_position.y - window_position.y - tab_size),
    };
}

titian::Float2 titian::GUISectionViewport::window_to_ndc(const Int2 coords) const
{
    const ImVec2 window_size = im::GetWindowContentRegionMax() - im::GetWindowContentRegionMin();
    return {
        2.0f * coords.x / window_size.x - 1.0f,
        2.0f * (window_size.y - coords.y) / window_size.y - 1.0f,
    };
}

titian::Set<uint32_t> titian::GUISectionViewport::read_id_texture(const Int2 first_coords, const Int2 last_coords)
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
    const Int2 size = max_coords - min_coords;

    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    Camera* main_camera = scene.get_casted<Camera>(scene.main_camera_name);
    if (!main_camera)
        return {};

    main_camera->resize_staging(size);
    gpu.copy_resource_region(main_camera->index_staging.texture,
        main_camera->index_texture.texture, min_coords, max_coords);

    Vector<float> values(size.x * size.y);
    gpu.read_from_texture(values.data(), main_camera->index_staging.texture, size, sizeof(float));

    Set<uint32_t> results;
    for (float value : values) {
        results.insert(uint32_t(value));
    }
    return results;
}

titian::Optional<titian::Float3> titian::GUISectionViewport::read_depth_texture(const Int2 coords)
{
    kl::GPU& gpu = AppLayer::get().gpu;
    Scene& scene = GameLayer::get().scene();

    Camera* main_camera = scene.get_casted<Camera>(scene.main_camera_name);
    if (!main_camera)
        return std::nullopt;

    gpu.copy_resource(main_camera->depth_staging.texture, main_camera->depth_texture.texture);

    float depth = 0.0f;
    gpu.map_read_resource(main_camera->depth_staging.texture, [&](const byte* ptr, const uint32_t pitch)
    {
        kl::copy<float>(&depth, ptr + coords.x * sizeof(float) + coords.y * pitch, 1);
    });
    if (depth <= 0.0f || depth >= 1.0f)
        return std::nullopt;

    Float4 position = { window_to_ndc(coords), depth, 1.0f };
    position = kl::inverse(main_camera->camera_matrix()) * position;
    return { position.xyz() / position.w };
}

void titian::GUISectionViewport::render_gizmos(const Set<Entity*>& entities)
{
	EditorLayer& editor_layer = EditorLayer::get();
	GameLayer& game_layer = GameLayer::get();
	AppLayer& app_layer = AppLayer::get();

    if (editor_layer.gizmo_operation == ImGuizmo::OPERATION::NONE)
        return;

    kl::Window& window = app_layer.window;
    Scene& scene = game_layer.scene();

    Camera* camera = scene.get_casted<Camera>(scene.main_camera_name);
    if (!camera)
        return;

    const float viewport_tab_height = im::GetWindowContentRegionMin().y;
    const Float2 viewport_position = { im::GetWindowPos().x, im::GetWindowPos().y + viewport_tab_height };
    const Float2 viewport_size = { im::GetWindowWidth(), im::GetWindowHeight() };

    ImGuizmo::Enable(true);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    Float3 selected_snap;
    if (window.keyboard.shift) {
        static constexpr Float3 predefined_snaps[3] = { Float3{ 0.1f }, Float3{ 15.0f }, Float3{ 1.0f } };
        switch (editor_layer.gizmo_operation)
        {
        case ImGuizmo::OPERATION::SCALE: selected_snap = predefined_snaps[0]; break;
        case ImGuizmo::OPERATION::ROTATE: selected_snap = predefined_snaps[1]; break;
        case ImGuizmo::OPERATION::TRANSLATE: selected_snap = predefined_snaps[2]; break;
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
        collective_center /= float(entities.size());
        transform_matrix = kl::transpose(Float4x4::translation(collective_center));
    }

    ImGuizmo::Manipulate(view_matrix.data, projection_matrix.data,
        editor_layer.gizmo_operation, editor_layer.gizmo_mode,
        transform_matrix.data, nullptr,
        &selected_snap.x);

    if (!ImGuizmo::IsUsing()) {
        m_last_scaling = Float3{ 1.0f };
        m_was_using = false;
        return;
    }
    if (!m_was_using) {
        m_was_using = true;
        if (im::IsKeyDown(ImGuiKey_LeftAlt)) {
            StringSet new_entities;
            for (const String& entity_name : editor_layer.selected_entities) {
                Entity* entity = scene.helper_get_entity(entity_name);
                if (!entity)
                    continue;

                const String new_name = Scene::generate_unique_name(entity_name, scene.entities());
                scene.add_entity(new_name, entity->clone());
                new_entities.insert(new_name);
            }
            editor_layer.selected_entities = new_entities;
            return;
        }
    }

    Float3 decomposed_parts[3] = {};
    ImGuizmo::DecomposeMatrixToComponents(transform_matrix.data,
        &decomposed_parts[2].x, &decomposed_parts[1].x, &decomposed_parts[0].x);

    if (entities.size() == 1) {
        Entity* entity = *entities.begin();
        entity->set_scale(decomposed_parts[0]);
        entity->set_rotation(decomposed_parts[1]);
        entity->set_position(decomposed_parts[2]);
    }
    else {
        Float4x4 position_transform;
        if (editor_layer.gizmo_operation == ImGuizmo::OPERATION::SCALE) {
            position_transform =
                Float4x4::translation(collective_center)
                * Float4x4::scaling(Float3{ 1.0f } + decomposed_parts[0] - m_last_scaling)
                * Float4x4::translation(-collective_center);
            m_last_scaling = decomposed_parts[0];
        }
        else {
            position_transform = kl::transpose(transform_matrix) * Float4x4::translation(-collective_center);
        }
        if (editor_layer.gizmo_mode == ImGuizmo::MODE::LOCAL) {
            for (Entity* entity : entities) {
                entity->set_rotation(entity->rotation() + Float3{ -decomposed_parts[1].x, -decomposed_parts[1].y, decomposed_parts[1].z });
            }
        }
        for (Entity* entity : entities) {
            entity->set_position((position_transform * Float4(entity->position(), 1.0f)).xyz());
        }
    }
}
