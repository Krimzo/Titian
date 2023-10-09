export module gui_material_editor;

export import gui_render;

void display_materials(EditorState* state);
void display_textures(EditorState* state);
void update_material_camera(EditorState* state);
void render_selected_material(EditorState* state, const kl::Object<kl::Material>& material, const kl::Int2& viewport_size);
void render_selected_texture(EditorState* state, const kl::Object<kl::Texture>& texture, const kl::Int2& viewport_size);
void show_material_info(EditorState* state, kl::Object<kl::Material>& material);
void show_texture_info(EditorState* state, kl::Object<kl::Texture>& texture);

export void gui_material_editor(EditorState* state)
{
    if (ImGui::Begin("Material Editor")) {
        const float available_width = ImGui::GetContentRegionAvail().x;
        ImGui::Columns(3, "MaterialEditorColumns", false);

        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Meshes")) {
            display_materials(state);
        }
        ImGui::EndChild();

        ImGui::NextColumn();
        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Textures")) {
            display_textures(state);
        }
        ImGui::EndChild();

        const std::optional texture_file_path = gui::drag_drop::read_data<std::string>("TextureFile");
        if (texture_file_path) {
            const std::filesystem::path path = texture_file_path.value();
            const std::string texture_name = path.filename().string();
            if (!state->scene->textures.contains(texture_name)) {
                const kl::TextureData texture_data {path.string()};
                kl::Object new_texture = new kl::Texture(&state->gpu, texture_data);
                state->scene->textures[texture_name] = new_texture;
                if (new_texture) {
                    new_texture->create_shader_view();
                }
            }
        }
        ImGui::NextColumn();

        kl::Object<kl::Material>& selected_material = state->gui_state->material_editor.selected_material;
        kl::Object<kl::Texture>& selected_texture = state->gui_state->material_editor.selected_texture;
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4&) state->gui_state->color_special);
        static bool was_focused = true;

        const bool should_rotate_cam = was_focused && !selected_texture;
        if (ImGui::BeginChild("Material/Texture View", {}, should_rotate_cam)) {
            const kl::Int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
            if (should_rotate_cam) update_material_camera(state);
            if (selected_material) {
                render_selected_material(state, selected_material, viewport_size);
                const kl::dx::ShaderView& shader_view = state->gui_state->material_editor.render_texture->shader_view;
                if (shader_view) ImGui::Image(shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
            }
            else if (selected_texture) {
                render_selected_texture(state, selected_texture, viewport_size);
            }
            was_focused = ImGui::IsWindowFocused();
        }
        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        if (selected_material) {
            show_material_info(state, selected_material);
        }
        else if (selected_texture) {
            show_texture_info(state, selected_texture);
        }
    }
    ImGui::End();
}

void display_materials(EditorState* state)
{
    kl::Object<kl::Material>& selected_material = state->gui_state->material_editor.selected_material;

    if (ImGui::BeginPopupContextWindow("CreateMaterial", ImGuiPopupFlags_MouseButtonRight)) {
        ImGui::Text("Create Material");

        char name_input[31] = {};
        if (ImGui::InputText("##CreateMaterialInput", name_input, std::size(name_input) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (!state->scene->materials.contains(name_input)) {
                const kl::Object material = new kl::Material();
                state->scene->materials[name_input] = material;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    for (const auto& [material_name, material] : state->scene->materials) {
        if (ImGui::Selectable(material_name.c_str(), material == selected_material)) {
            selected_material = material;
            state->gui_state->material_editor.selected_texture = nullptr;
        }

        if (ImGui::BeginPopupContextItem(material_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            ImGui::Text("Edit Material");

            char name_input[31] = {};
            memcpy(name_input, material_name.c_str(), std::min(material_name.size(), std::size(name_input) - 1));

            if (ImGui::InputText("##RenameMaterialInput", name_input, std::size(name_input) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (!state->scene->materials.contains(name_input)) {
                    state->scene->materials[name_input] = material;
                    state->scene->materials.erase(material_name);
                    ImGui::CloseCurrentPopup();
                    should_break = true;
                }
            }

            if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                if (material == selected_material) {
                    selected_material = nullptr;
                }
                for (auto& [_, entity] : *state->scene) {
                    if (entity->material == material) {
                        entity->material = nullptr;
                    }
                }
                state->scene->materials.erase(material_name);
                ImGui::CloseCurrentPopup();
                should_break = true;
            }

            ImGui::EndPopup();
            if (should_break) break;
        }
    }
}

void display_textures(EditorState* state)
{
    kl::Object<kl::Texture>& selected_texture = state->gui_state->material_editor.selected_texture;

    for (const auto& [texture_name, texture] : state->scene->textures) {
        if (ImGui::Selectable(texture_name.c_str(), texture == selected_texture)) {
            selected_texture = texture;
            state->gui_state->material_editor.selected_material = nullptr;
        }

        if (ImGui::BeginPopupContextItem(texture_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            ImGui::Text("Edit Texture");

            char name_input[31] = {};
            memcpy(name_input, texture_name.c_str(), std::min(texture_name.size(), std::size(name_input) - 1));

            if (ImGui::InputText("##RenameTextureInput", name_input, std::size(name_input) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (!state->scene->textures.contains(name_input)) {
                    state->scene->textures[name_input] = texture;
                    state->scene->textures.erase(texture_name);
                    ImGui::CloseCurrentPopup();
                    should_break = true;
                }
            }

            if (ImGui::Button("Delete", { -1.0f, 0.0f })) {
                if (texture == selected_texture) {
                    selected_texture = nullptr;
                }
                for (auto& [_, material] : state->scene->materials) {
                    if (material->color_map == texture) {
                        material->color_map = nullptr;
                    }
                    if (material->normal_map == texture) {
                        material->normal_map = nullptr;
                    }
                    if (material->roughness_map == texture) {
                        material->roughness_map = nullptr;
                    }
                }
                state->scene->textures.erase(texture_name);
                ImGui::CloseCurrentPopup();
                should_break = true;
            }

            ImGui::EndPopup();
            if (should_break) break;
        }
    }
}

void update_material_camera(EditorState* state)
{
    static kl::Float2 initial_camera_info = { 40, 30 };
    static kl::Float2 camera_info = initial_camera_info;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    kl::Camera& camera = state->gui_state->material_editor.camera;

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

void render_selected_material(EditorState* state, const kl::Object<kl::Material>& material, const kl::Int2& viewport_size)
{
    if (!state->gui_state->material_editor.render_texture) {
        state->gui_state->material_editor.render_texture = new kl::Texture(&state->gpu);
    }

    kl::dx::Texture& render_texture = state->gui_state->material_editor.render_texture->graphics_buffer;
    kl::dx::TargetView& target_view = state->gui_state->material_editor.render_texture->target_view;
    kl::dx::ShaderView& shader_view = state->gui_state->material_editor.render_texture->shader_view;

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
        state->gui_state->material_editor.render_texture->create_target_view();
        state->gui_state->material_editor.render_texture->create_shader_view();
    }

    state->gpu->bind_target_depth_views({ target_view }, state->gpu->internal_depth());
    state->gpu->clear_target_view(target_view, kl::Color { 30, 30, 30 });
    state->gpu->clear_internal_depth();
    state->gpu->set_viewport_size(viewport_size);

    state->gpu->bind_raster_state(state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.enabled);

    kl::RenderShaders& shaders = state->gui_state->material_editor.shaders;
    if (!shaders) {
        shaders = state->render_shaders.object_material;
    }
    state->gpu->bind_render_shaders(shaders);

    kl::Camera& camera = state->gui_state->material_editor.camera;
    camera.update_aspect_ratio(viewport_size);

    class VsData
    {
    public:
        kl::Float4x4  w_matrix;
        kl::Float4x4 vp_matrix;
    } vs_data = {};

    vs_data.w_matrix = {};
    vs_data.vp_matrix = camera.matrix();
    shaders.vertex_shader.update_cbuffer(vs_data);

    class PsData
    {
    public:
        kl::Float4 object_color; // (color.r, color.g, color.b, none)

        kl::Float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
        kl::Float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)

        kl::Float4 camera_info; // (camera.x, camera.y, camera.z, skybox?)
        kl::Float4 camera_background; // (color.r, color.g, color.b, color.a)

        kl::Float4     ambient_light; // (color.r, color.g, color.b, intensity)
        kl::Float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)
    } ps_data = {};

    if (kl::Object<kl::Texture>& skybox = state->scene->camera->skybox) {
        state->gpu->bind_shader_view_for_pixel_shader(skybox->shader_view, 0);
    }
    else {
        state->gpu->unbind_shader_view_for_pixel_shader(0);
    }

    if (material->color_map) {
        state->gpu->bind_shader_view_for_pixel_shader(material->color_map->shader_view, 1);
    }
    if (material->normal_map) {
        state->gpu->bind_shader_view_for_pixel_shader(material->normal_map->shader_view, 2);
        ps_data.object_texture_info.x = 1.0f;
    }
    else {
        ps_data.object_texture_info.x = 0.0f;
    }
    if (material->roughness_map) {
        state->gpu->bind_shader_view_for_pixel_shader(material->roughness_map->shader_view, 3);
        ps_data.object_texture_info.y = 1.0f;
    }
    else {
        ps_data.object_texture_info.y = 0.0f;
    }

    ps_data.ambient_light = { kl::Float3 {1.0f}, 0.1f };
    ps_data.directional_light = { kl::normalize(kl::Float3 { 0.0f, -1.0f, -1.0f }), 1.0f };

    ps_data.object_color = material->color;
    ps_data.object_material = {
        material->texture_blend,
        material->reflection_factor,
        material->refraction_factor,
        material->refraction_index,
    };
    ps_data.camera_info = { camera.origin, (float) (bool) state->scene->camera->skybox };
    ps_data.camera_background = state->scene->camera->background;
    shaders.pixel_shader.update_cbuffer(ps_data);

    state->gpu->draw(state->default_mesh.cube->graphics_buffer);
    state->gpu->bind_internal_views();
}

void render_selected_texture(EditorState* state, const kl::Object<kl::Texture>& texture, const kl::Int2& viewport_size)
{
    const float min_size = (float) std::min(viewport_size.x, viewport_size.y);
    ImGui::SetCursorPos(ImVec2 {
        (ImGui::GetWindowWidth() - min_size) * 0.5f,
        (ImGui::GetWindowHeight() - min_size) * 0.5f,
    });
    ImGui::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void show_material_info(EditorState* state, kl::Object<kl::Material>& material)
{
    if (ImGui::Begin("Material Info")) {
        ImGui::DragFloat("Texture Blend", &material->texture_blend, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Reflection Factor", &material->reflection_factor, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Refraction Factor", &material->refraction_factor, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Refraction Index", &material->refraction_index, 0.05f, 0.0f, 1.0f);

        ImGui::ColorEdit4("Base Color", material->color);

        std::string names[3] = { "/", "/", "/" };
        for (const auto& [texture_name, texture] : state->scene->textures) {
            if (texture == material->color_map) {
                names[0] = texture_name;
            }
            if (texture == material->normal_map) {
                names[1] = texture_name;
            }
            if (texture == material->roughness_map) {
                names[2] = texture_name;
            }
        }

        if (ImGui::BeginCombo("Color Map", names[0].c_str())) {
            if (ImGui::Selectable("/", !material->color_map)) {
                material->color_map = nullptr;
            }

            for (auto& [texture_name, texture] : state->scene->textures) {
                if (ImGui::Selectable(texture_name.c_str(), texture == material->color_map)) {
                    material->color_map = texture;
                }
            }

            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Normal Map", names[1].c_str())) {
            if (ImGui::Selectable("/", !material->normal_map)) {
                material->normal_map = nullptr;
            }

            for (auto& [texture_name, texture] : state->scene->textures) {
                if (ImGui::Selectable(texture_name.c_str(), texture == material->normal_map)) {
                    material->normal_map = texture;
                }
            }

            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Roughness Map", names[2].c_str())) {
            if (ImGui::Selectable("/", !material->roughness_map)) {
                material->roughness_map = nullptr;
            }

            for (auto& [texture_name, texture] : state->scene->textures) {
                if (ImGui::Selectable(texture_name.c_str(), texture == material->roughness_map)) {
                    material->roughness_map = texture;
                }
            }

            ImGui::EndCombo();
        }
    }
    ImGui::End();
}

void show_texture_info(EditorState* state, kl::Object<kl::Texture>& texture)
{
    if (ImGui::Begin("Texture Info")) {
        kl::Int2 size = texture->data_buffer.size();
        ImGui::DragInt2("Size", size, 0.0f);

        int pixel_count = size.x * size.y;
        ImGui::DragInt("Pixel Count", &pixel_count, 0.0f);

        bool cube_map = texture->is_cube;
        if (ImGui::Checkbox("Cube Map", &cube_map)) {
            if (cube_map) {
                if (texture->reload_as_cube())
                    texture->create_shader_view();
            }
            else {
                texture->reload();
                texture->create_shader_view();
            }
        }
    }
    ImGui::End();
}