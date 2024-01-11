#include "main.h"


titian::GUISectionMaterialEditor::GUISectionMaterialEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
{
    this->editor_layer = editor_layer;
    this->gui_layer = gui_layer;

    kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
    Scene* scene = &editor_layer->game_layer->scene;

    camera = new Camera(scene->physics(), true);
    render_texture = new Texture(gpu);
    depth_texture = new Texture(gpu);

    camera->background = kl::Color{ 30, 30, 30 };
    camera->set_position({ 0.642787576f, 0.577350259f, 0.766044438f });
    camera->speed = 3.1f;
}

void titian::GUISectionMaterialEditor::render_gui()
{
    kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
    Scene* scene = &editor_layer->game_layer->scene;
    Material* material = &scene->get_material(this->selected_material);
    Texture* texture = &scene->get_texture(this->selected_texture);

    if (ImGui::Begin("Material Editor")) {
        const float available_width = ImGui::GetContentRegionAvail().x;
        ImGui::Columns(3, "MaterialEditorColumns", false);

        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Materials")) {
            display_materials(scene);
        }
        ImGui::EndChild();

        ImGui::NextColumn();
        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Textures")) {
            display_textures(scene);
        }
        ImGui::EndChild();

        const std::optional texture_file_path = gui_get_drag_drop<std::string>("TextureFile");
        if (texture_file_path) {
            const std::filesystem::path path = texture_file_path.value();
            const std::string texture_name = path.filename().string();
            if (!scene->textures.contains(texture_name)) {
                kl::Object new_texture = new Texture(gpu);
                new_texture->data_buffer.load_from_file(path.string());
                new_texture->load_as_2D(false, false);
                new_texture->create_shader_view(nullptr);
                scene->textures[texture_name] = new_texture;
            }
        }
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        const bool should_rotate_cam = was_focused && !texture;
        if (ImGui::BeginChild("Material/Texture View", {}, should_rotate_cam)) {
            const kl::Int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
            if (should_rotate_cam) {
                update_material_camera();
            }
            if (material) {
                render_selected_material(scene, gpu, material, viewport_size);
                ImGui::Image(render_texture->shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
            }
            else if (texture) {
                render_selected_texture(texture, viewport_size);
            }
            was_focused = ImGui::IsWindowFocused();
        }
        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        if (material) {
            show_material_properties(scene, material);
        }
        else if (texture) {
            show_texture_properties(texture);
        }
    }
    ImGui::End();
}

void titian::GUISectionMaterialEditor::display_materials(Scene* scene)
{
    kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;

    // New material
    if (ImGui::BeginPopupContextWindow("NewMaterial", ImGuiPopupFlags_MouseButtonMiddle)) {
        ImGui::Text("New Material");

        if (std::optional opt_name = gui_input_waited("##CreateMaterialInput", {})) {
            const std::string& name = opt_name.value();
            if (!name.empty() && !scene->materials.contains(name)) {
                kl::Object material = new Material(gpu);
                scene->materials[name] = material;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    // Materials
    const std::string filter = gui_input_continuous("Search###MeterialEditor");
    for (const auto& [material_name, material] : scene->materials) {
        if (!filter.empty() && material_name.find(filter) == -1) {
            continue;
        }

        if (ImGui::Selectable(material_name.c_str(), material_name == this->selected_material)) {
            this->selected_material = material_name;
            this->selected_texture = "/";
        }

        if (ImGui::BeginPopupContextItem(material_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            ImGui::Text("Edit Material");

            if (std::optional opt_name = gui_input_waited("##RenameMaterialInput", material_name)) {
                const std::string& name = opt_name.value();
                if (!name.empty() && !scene->materials.contains(name)) {
                    for (auto& [_, entity] : *scene) {
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
                    ImGui::CloseCurrentPopup();
                }
            }

            if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                for (auto& [_, entity] : *scene) {
                    if (entity->material_name == material_name) {
                        entity->material_name = "/";
                    }
                }
                if (this->selected_material == material_name) {
                    selected_material = "/";
                }
                scene->materials.erase(material_name);
                should_break = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionMaterialEditor::display_textures(Scene* scene)
{
    const std::string filter = gui_input_continuous("Search###TextureEditor");
    for (const auto& [texture_name, texture] : scene->textures) {
        if (!filter.empty() && texture_name.find(filter) == -1) {
            continue;
        }

        if (ImGui::Selectable(texture_name.c_str(), texture_name == this->selected_texture)) {
            this->selected_texture = texture_name;
            this->selected_material = "/";
        }

        if (ImGui::BeginPopupContextItem(texture_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            ImGui::Text("Edit Texture");

            if (std::optional opt_name = gui_input_waited("##RenameTextureInput", texture_name)) {
                const std::string& name = opt_name.value();
                if (!name.empty() && !scene->textures.contains(name)) {
                    for (auto& [_, material] : scene->materials) {
                        if (material->color_map_name == texture_name) {
                            material->color_map_name = name;
                        }
                        if (material->normal_map_name == texture_name) {
                            material->normal_map_name = name;
                        }
                        if (material->roughness_map_name == texture_name) {
                            material->roughness_map_name = name;
                        }
                    }
                    if (this->selected_texture == texture_name) {
                        this->selected_texture = name;
                    }
                    scene->textures[name] = texture;
                    scene->textures.erase(texture_name);
                    should_break = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                for (auto& [_, material] : scene->materials) {
                    if (material->color_map_name == texture_name) {
                        material->color_map_name = "/";
                    }
                    if (material->normal_map_name == texture_name) {
                        material->normal_map_name = "/";
                    }
                    if (material->roughness_map_name == texture_name) {
                        material->roughness_map_name = "/";
                    }
                }
                if (this->selected_texture == texture_name) {
                    this->selected_texture = "/";
                }
                scene->textures.erase(texture_name);
                should_break = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionMaterialEditor::update_material_camera()
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    const int scroll = editor_layer->game_layer->app_layer->window->mouse.scroll();
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        const ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
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

void titian::GUISectionMaterialEditor::render_selected_material(Scene* scene, kl::GPU* gpu, Material* material, const kl::Int2 viewport_size)
{
    // Don't render if the viewport is too small
    if (viewport_size.x <= 0 || viewport_size.y <= 0) {
        return;
    }

    // Create render texture if needed
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

    // Bind render target
    gpu->bind_target_depth_views({ render_texture->target_view }, depth_texture->depth_view);
    gpu->clear_target_view(render_texture->target_view, camera->background);
    gpu->clear_depth_view(depth_texture->depth_view, 1.0f, 0xFF);

    // Set viewport
    const kl::Int2 old_viewport_size = gpu->viewport_size();
    gpu->set_viewport_size(viewport_size);

    // Bind states
    RenderStates* states = &gui_layer->render_layer->states;
    gpu->bind_raster_state(states->raster_states->solid);
    gpu->bind_depth_state(material->is_transparent() ? states->depth_states->only_compare : states->depth_states->enabled);
    gpu->bind_blend_state(states->blend_states->enabled);

    // Bind shaders
    kl::RenderShaders* render_shaders = material->shaders ? &material->shaders : &states->shader_states->scene_pass;
    gpu->bind_render_shaders(*render_shaders);

    // Bind camera
    camera->update_aspect_ratio(viewport_size);
    Camera* scene_camera = scene->get_casted<Camera>(scene->main_camera_name);
    if (!scene_camera) {
        return;
    }

    // Set global constants
    struct GLOBAL_CB
    {
        float ELAPSED_TIME{};
        float DELTA_TIME{};

        alignas(16) kl::Float3 CAMERA_POSITION;
        float CAMERA_HAS_SKYBOX{};
        kl::Float4 CAMERA_BACKGROUND;

        kl::Float3 AMBIENT_COLOR;
        float AMBIENT_INTENSITY{};

        kl::Float3 SUN_DIRECTION;
        float SUN_POINT_SIZE{};
        kl::Float3 SUN_COLOR;

        float OBJECT_INDEX{};
        kl::Float3 OBJECT_SCALE;
        alignas(16) kl::Float3 OBJECT_ROTATION;
        alignas(16) kl::Float3 OBJECT_POSITION;

        alignas(16) kl::Float3 OBJECT_COLOR;
        float ALPHA_BLEND{};
        float TEXTURE_BLEND{};

        float REFLECTION_FACTOR{};
        float REFRACTION_FACTOR{};
        float REFRACTION_INDEX{};

        float HAS_NORMAL_MAP{};
        float HAS_ROUGHNESS_MAP{};

        alignas(16) kl::Float4x4 W;
        kl::Float4x4 V;
        kl::Float4x4 VP;

        float RECEIVES_SHADOWS{};
        kl::Float2 SHADOW_MAP_SIZE;
        alignas(16) kl::Float2 SHADOW_MAP_TEXEL_SIZE;
        alignas(16) kl::Float4 SHADOW_CASCADES;
        kl::Float4x4 LIGHT_VPs[DirectionalLight::CASCADE_COUNT];
    };
    GLOBAL_CB global_cb{};

    const kl::Timer* timer = &editor_layer->game_layer->app_layer->timer;
    global_cb.ELAPSED_TIME = timer->elapsed();
    global_cb.DELTA_TIME = timer->delta();

    global_cb.W = {};
    global_cb.VP = camera->camera_matrix();

    // Bind textures
    Texture* skybox_texture = &scene->get_texture(scene_camera->skybox_name);
    if (skybox_texture) {
        gpu->bind_shader_view_for_pixel_shader(skybox_texture->shader_view, 0);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(0);
    }

    Texture* color_map_texture = &scene->get_texture(material->color_map_name);
    if (color_map_texture) {
        gpu->bind_shader_view_for_pixel_shader(color_map_texture->shader_view, 1);
    }
    else {
        gpu->unbind_shader_view_for_pixel_shader(1);
    }

    Texture* normal_map_texture = &scene->get_texture(material->normal_map_name);
    if (normal_map_texture) {
        gpu->bind_shader_view_for_pixel_shader(normal_map_texture->shader_view, 2);
        global_cb.HAS_NORMAL_MAP = 1.0f;
    }
    else {
        global_cb.HAS_NORMAL_MAP = 0.0f;
    }

    Texture* roughness_map_texture = &scene->get_texture(material->roughness_map_name);
    if (roughness_map_texture) {
        gpu->bind_shader_view_for_pixel_shader(roughness_map_texture->shader_view, 3);
        global_cb.HAS_ROUGHNESS_MAP = 1.0f;
    }
    else {
        global_cb.HAS_ROUGHNESS_MAP = 0.0f;
    }

    // Set global constants
    global_cb.CAMERA_POSITION = camera->position();
    global_cb.CAMERA_HAS_SKYBOX = static_cast<float>(static_cast<bool>(skybox_texture));
    global_cb.CAMERA_BACKGROUND = camera->background;

    global_cb.AMBIENT_COLOR = kl::Float3{ 1.0f };
    global_cb.AMBIENT_INTENSITY = 0.1f;

    global_cb.SUN_DIRECTION = kl::normalize(kl::Float3{ 0.0f, -1.0f, -1.0f });
    global_cb.SUN_COLOR = kl::colors::WHITE;

    global_cb.OBJECT_COLOR = material->color.xyz();
    global_cb.ALPHA_BLEND = material->alpha_blend;
    global_cb.TEXTURE_BLEND = material->texture_blend;

    global_cb.REFLECTION_FACTOR = material->reflection_factor;
    global_cb.REFRACTION_FACTOR = material->refraction_factor;
    global_cb.REFRACTION_INDEX = material->refraction_index;

    global_cb.RECEIVES_SHADOWS = false;

    render_shaders->vertex_shader.update_cbuffer(global_cb);
    render_shaders->pixel_shader.update_cbuffer(global_cb);

    // Draw mesh
    DefaultMeshes* default_meshes = &editor_layer->game_layer->scene->default_meshes;
    gpu->draw(default_meshes->cube->graphics_buffer, default_meshes->cube->casted_topology());

    // Restore
    gpu->bind_internal_views();
    gpu->set_viewport_size(old_viewport_size);
}

void titian::GUISectionMaterialEditor::render_selected_texture(Texture* texture, const kl::Int2 viewport_size)
{
    const float min_size = static_cast<float>(std::min(viewport_size.x, viewport_size.y));
    ImGui::SetCursorPos(ImVec2{
        (ImGui::GetWindowWidth() - min_size) * 0.5f,
        (ImGui::GetWindowHeight() - min_size) * 0.5f,
        });
    ImGui::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void titian::GUISectionMaterialEditor::show_material_properties(Scene* scene, Material* material)
{
    if (ImGui::Begin("Material Properties") && material) {
        ImGui::Text("Info");

        ImGui::Text("Name: ");
        ImGui::SameLine();
        gui_colored_text(selected_material, gui_layer->special_color);

        ImGui::DragFloat("Alpha Blend", &material->alpha_blend, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Texture Blend", &material->texture_blend, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Reflection Factor", &material->reflection_factor, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Refraction Factor", &material->refraction_factor, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Refraction Index", &material->refraction_index, 0.05f, 0.0f, 1.0f);

        ImGui::ColorEdit4("Base Color", material->color);

        if (ImGui::BeginCombo("Color Map", material->color_map_name.c_str())) {
            if (ImGui::Selectable("/", material->color_map_name == "/")) {
                material->color_map_name = "/";
            }
            for (auto& [texture_name, _] : scene->textures) {
                if (ImGui::Selectable(texture_name.c_str(), texture_name == material->color_map_name)) {
                    material->color_map_name = texture_name;
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Normal Map", material->normal_map_name.c_str())) {
            if (ImGui::Selectable("/", material->normal_map_name == "/")) {
                material->normal_map_name = "/";
            }
            for (auto& [texture_name, _] : scene->textures) {
                if (ImGui::Selectable(texture_name.c_str(), texture_name == material->normal_map_name)) {
                    material->normal_map_name = texture_name;
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Roughness Map", material->roughness_map_name.c_str())) {
            if (ImGui::Selectable("/", material->roughness_map_name == "/")) {
                material->roughness_map_name = "/";
            }
            for (auto& [texture_name, _] : scene->textures) {
                if (ImGui::Selectable(texture_name.c_str(), material->roughness_map_name == texture_name)) {
                    material->roughness_map_name = texture_name;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        if (ImGui::Button("Reload")) {
            material->reload();
        }

        char source_file_buffer[151] = {};
        memcpy(source_file_buffer, material->shader_source_file.c_str(), std::min(material->shader_source_file.size(), std::size(source_file_buffer) - 1));
        if (ImGui::InputText("Shader Source File", source_file_buffer, std::size(source_file_buffer))) {
            material->shader_source_file = source_file_buffer;
        }
        if (std::optional shader_file = gui_get_drag_drop<std::string>("ShaderFile")) {
            material->shader_source_file = shader_file.value();
        }
        ImGui::InputTextMultiline("Shader Source", material->shader_source.data(), material->shader_source.size(), {}, ImGuiInputTextFlags_ReadOnly);
    }
    ImGui::End();
}

void titian::GUISectionMaterialEditor::show_texture_properties(Texture* texture)
{
    if (ImGui::Begin("Texture Properties") && texture) {
        ImGui::Text("Info");

        ImGui::Text("Name: ");
        ImGui::SameLine();
        gui_colored_text(selected_texture, gui_layer->special_color);

        kl::Int2 size = texture->data_buffer.size();
        ImGui::DragInt2("Size", size, 0.0f);

        int pixel_count = texture->data_buffer.pixel_count();
        ImGui::DragInt("Pixel Count", &pixel_count, 0.0f);

        bool cube_map = texture->is_cube();
        if (ImGui::Checkbox("Cube Map", &cube_map)) {
            if (cube_map) {
                if (texture->load_as_cube()) {
                    texture->create_shader_view(nullptr);
                }
            }
            else {
                texture->load_as_2D(false, false);
                texture->create_shader_view(nullptr);
            }
        }
    }
    ImGui::End();
}
