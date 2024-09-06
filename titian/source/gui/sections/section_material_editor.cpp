#include "titian.h"


titian::GUISectionMaterialEditor::GUISectionMaterialEditor()
    : GUISection("GUISectionMaterialEditor")
{
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    camera = new Camera(scene->physics(), true);
    render_texture = new Texture(gpu);
    depth_texture = new Texture(gpu);

    camera->background = Color{ 30, 30, 30 };
    camera->set_position({ 0.642787576f, 0.577350259f, 0.766044438f });
    camera->speed = 3.1f;
}

void titian::GUISectionMaterialEditor::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
    Scene* scene = &Layers::get<GameLayer>()->scene;

    Ref<Material> material;
    if (scene->materials.contains(this->selected_material)) {
        material = scene->materials.at(this->selected_material);
    }

    if (im::Begin("Material Editor")) {
        const float available_width = im::GetContentRegionAvail().x;
        im::Columns(2, "MaterialEditorColumns", false);

        im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
        if (im::BeginChild("Materials")) {
            display_materials(gpu, scene);
        }
        im::EndChild();
        im::NextColumn();

        im::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        im::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        im::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        const bool should_rotate_cam = was_focused;
        if (im::BeginChild("Material View", {}, should_rotate_cam)) {
            const Int2 viewport_size = { (int) im::GetContentRegionAvail().x, (int) im::GetContentRegionAvail().y };
            if (should_rotate_cam) {
                update_material_camera();
            }
            if (material) {
                render_selected_material(scene, gpu, &material, viewport_size);
                im::Image(render_texture->shader_view.get(), { (float) viewport_size.x, (float) viewport_size.y });
            }
            was_focused = im::IsWindowFocused();
        }
        im::EndChild();

        im::PopStyleColor();
        im::PopStyleVar(2);

        if (material) {
            show_material_properties(scene, &material);
        }
    }
    im::End();
}

void titian::GUISectionMaterialEditor::display_materials(kl::GPU* gpu, Scene* scene)
{
    // New material
    if (im::BeginPopupContextWindow("NewMaterial", ImGuiPopupFlags_MouseButtonMiddle)) {
        im::Text("New Material");

        if (Optional opt_name = gui_input_waited("##CreateMaterialInput", {})) {
            const auto& name = opt_name.value();
            if (!name.empty() && !scene->materials.contains(name)) {
                Ref material = new Material();
                scene->materials[name] = material;
                im::CloseCurrentPopup();
            }
        }
        im::EndPopup();
    }

    // Materials
    const String filter = gui_input_continuous("Search###MeterialEditor");
    for (auto& [material_name, material] : scene->materials) {
        if (!filter.empty() && material_name.find(filter) == -1) {
            continue;
        }

        if (im::Selectable(material_name.data(), material_name == this->selected_material)) {
            this->selected_material = material_name;
        }

        if (im::BeginPopupContextItem(material_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            im::Text("Edit Material");

            if (Optional opt_name = gui_input_waited("##RenameMaterialInput", material_name)) {
                const auto& name = opt_name.value();
                if (!name.empty() && !scene->materials.contains(name)) {
                    for (const auto& [_, entity] : scene->entities()) {
                        if (entity->material_name == material_name) {
                            entity->material_name = name;
                        }
                    }
                    if (this->selected_material == material_name) {
                        this->selected_material = name;
                    }
                    scene->materials[name] = material;
                    scene->materials.erase(material_name);
                    should_break = true;
                    im::CloseCurrentPopup();
                }
            }

            if (im::Button("Delete", { -1.0f, 0.0f })) {
                for (const auto& [_, entity] : scene->entities()) {
                    if (entity->material_name == material_name) {
                        entity->material_name = "/";
                    }
                }
                if (this->selected_material == material_name) {
                    selected_material = "/";
                }
                scene->materials.erase(material_name);
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

void titian::GUISectionMaterialEditor::render_selected_material(Scene* scene, kl::GPU* gpu, Material* material, const Int2 viewport_size)
{
    // Don't render if the viewport is too small
    if (viewport_size.x <= 0 || viewport_size.y <= 0) {
        return;
    }

    // Bind camera
    camera->update_aspect_ratio(viewport_size);
    Camera* scene_camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!scene_camera) {
        return;
    }

    AppLayer* app_layer = Layers::get<AppLayer>();
    RenderLayer* render_layer = Layers::get<RenderLayer>();
    GameLayer* game_layer = Layers::get<GameLayer>();

    // Create render texture if needed
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

    // Bind render target
    gpu->bind_target_depth_view(render_texture->target_view, depth_texture->depth_view);
    gpu->clear_target_view(render_texture->target_view, camera->background);
    gpu->clear_depth_view(depth_texture->depth_view, 1.0f, 0xFF);

    // Set viewport
    const Int2 old_viewport_size = gpu->viewport_size();
    gpu->set_viewport_size(viewport_size);

    // Bind states
    gpu->bind_raster_state(render_layer->raster_states->solid);
    gpu->bind_depth_state(material->is_transparent() ? render_layer->depth_states->only_compare : render_layer->depth_states->enabled);
    gpu->bind_blend_state(render_layer->blend_states->enabled);

    // Set global constants
    struct GLOBAL_CB
    {
        float ELAPSED_TIME{};
        float DELTA_TIME{};

        alignas(16) Float3 CAMERA_POSITION;
        float CAMERA_HAS_SKYBOX{};
        Float4 CAMERA_BACKGROUND;

        Float3 AMBIENT_COLOR;
        float AMBIENT_INTENSITY{};

        Float3 SUN_DIRECTION;
        float SUN_POINT_SIZE{};
        Float3 SUN_COLOR;

        float OBJECT_INDEX{};
        Float3 OBJECT_SCALE;
        alignas(16) Float3 OBJECT_ROTATION;
        alignas(16) Float3 OBJECT_POSITION;

        alignas(16) Float4 OBJECT_COLOR;
        float TEXTURE_BLEND{};

        float REFLECTION_FACTOR{};
        float REFRACTION_FACTOR{};
        float REFRACTION_INDEX{};

        float HAS_NORMAL_MAP{};
        float HAS_ROUGHNESS_MAP{};

        alignas(16) Float4x4 W;
        Float4x4 V;
        Float4x4 VP;

        float RECEIVES_SHADOWS{};
        Float2 SHADOW_MAP_SIZE;
        alignas(16) Float2 SHADOW_MAP_TEXEL_SIZE;
        alignas(16) Float4 SHADOW_CASCADES;
        Float4x4 LIGHT_VPs[DirectionalLight::CASCADE_COUNT];

        Float4x4 CUSTOM_DATA;
    };
    GLOBAL_CB global_cb{};

    const kl::Timer* timer = &app_layer->timer;
    global_cb.ELAPSED_TIME = timer->elapsed();
    global_cb.DELTA_TIME = timer->delta();

    global_cb.W = {};
    global_cb.VP = camera->camera_matrix();

    // Bind textures
    Texture* skybox_texture = scene->helper_get_texture(scene_camera->skybox_name);
    if (skybox_texture) {
        gpu->bind_shader_view_for_pixel_shader(skybox_texture->shader_view, 0);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(0);
    }

    Texture* color_map_texture = scene->helper_get_texture(material->color_map_name);
    if (color_map_texture) {
        gpu->bind_shader_view_for_pixel_shader(color_map_texture->shader_view, 5);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(5);
    }

    Texture* normal_map_texture = scene->helper_get_texture(material->normal_map_name);
    if (normal_map_texture) {
        gpu->bind_shader_view_for_pixel_shader(normal_map_texture->shader_view, 6);
        global_cb.HAS_NORMAL_MAP = 1.0f;
    }
    else {
        global_cb.HAS_NORMAL_MAP = 0.0f;
    }

    Texture* roughness_map_texture = scene->helper_get_texture(material->roughness_map_name);
    if (roughness_map_texture) {
        gpu->bind_shader_view_for_pixel_shader(roughness_map_texture->shader_view, 7);
        global_cb.HAS_ROUGHNESS_MAP = 1.0f;
    }
    else {
        global_cb.HAS_ROUGHNESS_MAP = 0.0f;
    }

    // Set global constants
    global_cb.CAMERA_POSITION = camera->position();
    global_cb.CAMERA_HAS_SKYBOX = static_cast<float>(static_cast<bool>(skybox_texture));
    global_cb.CAMERA_BACKGROUND = camera->background;

    global_cb.AMBIENT_COLOR = Float3{ 1.0f };
    global_cb.AMBIENT_INTENSITY = 0.1f;

    global_cb.SUN_DIRECTION = kl::normalize(Float3{ 0.0f, -1.0f, 1.0f });
    global_cb.SUN_COLOR = kl::colors::WHITE;

    global_cb.OBJECT_COLOR = material->color;
    global_cb.TEXTURE_BLEND = material->texture_blend;

    global_cb.REFLECTION_FACTOR = material->reflection_factor;
    global_cb.REFRACTION_FACTOR = material->refraction_factor;
    global_cb.REFRACTION_INDEX = material->refraction_index;

    global_cb.CUSTOM_DATA = material->custom_data;

    global_cb.RECEIVES_SHADOWS = false;

    kl::RenderShaders* render_shaders = &render_layer->shader_states->scene_pass;
    if (Shader* shader = scene->helper_get_shader(material->shader_name)) {
        render_shaders = &shader->graphics_buffer;
    }
    if (render_shaders && *render_shaders) {
        render_shaders->vertex_shader.update_cbuffer(global_cb);
        render_shaders->pixel_shader.update_cbuffer(global_cb);
        gpu->bind_render_shaders(*render_shaders);

        DefaultMeshes* default_meshes = &game_layer->scene->default_meshes;
        Mesh* material_mesh = &default_meshes->sphere;

        /* NOTE: shaders expect a bound target at index 1 (entity indices), we can discard these here */
        gpu->draw(material_mesh->graphics_buffer, material_mesh->casted_topology(), sizeof(Vertex));
    }

    // Restore
    gpu->bind_internal_views();
    gpu->set_viewport_size(old_viewport_size);
}

void titian::GUISectionMaterialEditor::show_material_properties(Scene* scene, Material* material)
{
    if (im::Begin("Material Properties") && material) {
        im::Text("Info");

        im::Text("Name: ");
        im::SameLine();
        gui_colored_text(selected_material, Layers::get<GUILayer>()->special_color);

        im::ColorEdit4("Base Color", &material->color.x);
        im::DragFloat("Texture Blend", &material->texture_blend, 0.05f, 0.0f, 1.0f);

        im::DragFloat("Reflection Factor", &material->reflection_factor, 0.05f, 0.0f, 1.0f);
        im::DragFloat("Refraction Factor", &material->refraction_factor, 0.05f, 0.0f, 1.0f);
        im::DragFloat("Refraction Index", &material->refraction_index, 0.05f, 0.0f, 1.0f);

        if (im::BeginCombo("Color Map", material->color_map_name.data())) {
            if (im::Selectable("/", material->color_map_name == "/")) {
                material->color_map_name = "/";
            }
            for (auto& [texture_name, _] : scene->textures) {
                if (im::Selectable(texture_name.data(), texture_name == material->color_map_name)) {
                    material->color_map_name = texture_name;
                }
            }
            im::EndCombo();
        }

        if (im::BeginCombo("Normal Map", material->normal_map_name.data())) {
            if (im::Selectable("/", material->normal_map_name == "/")) {
                material->normal_map_name = "/";
            }
            for (auto& [texture_name, _] : scene->textures) {
                if (im::Selectable(texture_name.data(), texture_name == material->normal_map_name)) {
                    material->normal_map_name = texture_name;
                }
            }
            im::EndCombo();
        }

        if (im::BeginCombo("Roughness Map", material->roughness_map_name.data())) {
            if (im::Selectable("/", material->roughness_map_name == "/")) {
                material->roughness_map_name = "/";
            }
            for (auto& [texture_name, _] : scene->textures) {
                if (im::Selectable(texture_name.data(), material->roughness_map_name == texture_name)) {
                    material->roughness_map_name = texture_name;
                }
            }
            im::EndCombo();
        }

        if (im::BeginCombo("Shader", material->shader_name.data())) {
            if (im::Selectable("/", material->shader_name == "/")) {
                material->shader_name = "/";
            }
            for (auto& [shader_name, shader] : scene->shaders) {
                if (shader->shader_type != ShaderType::MATERIAL) {
                    continue;
                }
                if (im::Selectable(shader_name.data(), material->shader_name == shader_name)) {
                    material->shader_name = shader_name;
                }
            }
            im::EndCombo();
        }
    }
    im::End();
}

void titian::GUISectionMaterialEditor::update_material_camera()
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
