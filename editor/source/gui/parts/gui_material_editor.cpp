#include "gui/gui_render.h"


void display_materials(editor_state* state);
void display_textures(editor_state* state);
void update_material_camera(editor_state* state);
void render_selected_material(editor_state* state, const kl::object<kl::material>& material, const kl::int2& viewport_size);
void render_selected_texture(editor_state* state, const kl::object<kl::texture>& texture, const kl::int2& viewport_size);
void show_material_info(editor_state* state, const kl::object<kl::material>& material);
void show_texture_info(editor_state* state, const kl::object<kl::texture>& texture);

void gui_material_editor(editor_state* state)
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

        std::optional texture_file_path = GUI::drag_drop::read_data<std::string>("TextureFile");
        if (texture_file_path) {
            const std::filesystem::path path = texture_file_path.value();
            const std::string texture_name = path.filename().string();
            if (!state->scene->textures.contains(texture_name)) {
                kl::texture_data texture_data = kl::texture_data(path.string());
                kl::object new_texture = new kl::texture(&state->gpu, texture_data);
                state->scene->textures[texture_name] = new_texture;
            }
        }
        ImGui::NextColumn();

        kl::object<kl::material>& selected_material = state->gui_state->material_editor.selected_material;
        kl::object<kl::texture>& selected_texture = state->gui_state->material_editor.selected_texture;
        
        if (ImGui::BeginChild("Material/Texture View")) {
            const kl::int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
            if (selected_material) {
                update_material_camera(state);
                render_selected_material(state, selected_material, viewport_size);

                kl::dx::shader_view& shader_view = state->gui_state->material_editor.render_texture->shader_view;
                if (shader_view) ImGui::Image(shader_view.Get(), { (float) viewport_size.x, (float) viewport_size.y });
            }
            else if (selected_texture) {
                render_selected_texture(state, selected_texture, viewport_size);
            }
        }
        ImGui::EndChild();

        if (selected_material) {
            show_material_info(state, selected_material);
        }
        else if (selected_texture) {
            show_texture_info(state, selected_texture);
        }
    }
    ImGui::End();
}

void display_materials(editor_state* state)
{
    kl::object<kl::material>& selected_material = state->gui_state->material_editor.selected_material;

    for (const auto& [material_name, material] : state->default_materials) {
        if (ImGui::Selectable(material_name.c_str(), material == selected_material)) {
            selected_material = material;
        }
    }
    ImGui::Separator();

    for (const auto& [material_name, material] : state->scene->materials) {
        if (ImGui::Selectable(material_name.c_str(), material == selected_material)) {
            selected_material = material;
            state->gui_state->material_editor.selected_texture = nullptr;
        }

        if (ImGui::BeginPopupContextItem(material_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;

            char name_input[31] = {};
            memcpy(name_input, material_name.c_str(), min(material_name.size(), std::size(name_input) - 1));

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

void display_textures(editor_state* state)
{
    kl::object<kl::texture>& selected_texture = state->gui_state->material_editor.selected_texture;

    for (const auto& [texture_name, texture] : state->scene->textures) {
        if (ImGui::Selectable(texture_name.c_str(), texture == selected_texture)) {
            selected_texture = texture;
            state->gui_state->material_editor.selected_material = nullptr;
        }

        if (ImGui::BeginPopupContextItem(texture_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;

            char name_input[31] = {};
            memcpy(name_input, texture_name.c_str(), min(texture_name.size(), std::size(name_input) - 1));

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

void update_material_camera(editor_state* state)
{
    static kl::float2 initial_camera_info = {};
    static kl::float2 camera_info = {};

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        initial_camera_info = camera_info;
    }

    kl::camera& camera = state->gui_state->material_editor.camera;

    static bool first_run = true;
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) || first_run) {
        const ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        camera_info.x = initial_camera_info.x + drag_delta.x * camera.sensitivity;
        camera_info.y = initial_camera_info.y + drag_delta.y * camera.sensitivity;
        camera_info.y = min(max(camera_info.y, -85.0f), 85.0f);

        camera.origin.x = sin(camera_info.x * kl::to_radians);
        camera.origin.z = cos(camera_info.x * kl::to_radians);
        camera.origin.y = tan(camera_info.y * kl::to_radians);
    }
    first_run = false;

    static int last_scroll = 0;
    const int scroll = state->window->mouse.scroll();
    camera.speed += (last_scroll - scroll) * 0.1f;
    camera.speed = max(camera.speed, 0.1f);
    last_scroll = scroll;

    const float camera_distance = camera.speed;
    camera.origin = kl::normalize(camera.origin) * camera_distance;
    camera.set_forward(-camera.origin);
}

void render_selected_material(editor_state* state, const kl::object<kl::material>& material, const kl::int2& viewport_size)
{
    if (!state->gui_state->material_editor.render_texture) {
        state->gui_state->material_editor.render_texture = new kl::texture(&state->gpu);
    }

    kl::dx::texture& render_texture = state->gui_state->material_editor.render_texture->graphics_buffer;
    kl::dx::target_view& target_view = state->gui_state->material_editor.render_texture->target_view;
    kl::dx::shader_view& shader_view = state->gui_state->material_editor.render_texture->shader_view;

    if (viewport_size.x <= 0 || viewport_size.y <= 0) {
        return;
    }

    kl::int2 texture_size = {};
    if (render_texture) {
        kl::dx::texture_descriptor descriptor = {};
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
    state->gpu->clear_target_view(target_view, kl::color { 30, 30, 30 });
    state->gpu->clear_internal_depth();
    state->gpu->set_viewport_size(viewport_size);

    state->gpu->bind_raster_state(state->raster_states.solid);
    state->gpu->bind_depth_state(state->depth_states.enabled);

    kl::render_shaders& shaders = state->gui_state->material_editor.shaders;
    if (!shaders) {
        shaders = state->render_shaders.object_material;
    }
    state->gpu->bind_render_shaders(shaders);

    kl::camera& camera = state->gui_state->material_editor.camera;
    camera.update_aspect_ratio(viewport_size);

    struct VS_DATA
    {
        kl::float4x4  w_matrix;
        kl::float4x4 vp_matrix;
    } vs_data = {};

    vs_data.w_matrix = {};
    vs_data.vp_matrix = camera.matrix();
    shaders.vertex_shader.update_cbuffer(vs_data);

    struct PS_DATA
    {
        kl::float4 object_color; // (color.r, color.g, color.b, none)

        kl::float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
        kl::float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)

        kl::float4 camera_info; // (camera.x, camera.y, camera.z, none)

        kl::float4     ambient_light; // (color.r, color.g, color.b, intensity)
        kl::float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)
    } ps_data = {};

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

    if (state->scene->ambient_light) {
        ps_data.ambient_light = { kl::float3 {1.0f}, 0.1f };
    }
    if (state->scene->directional_light) {
        ps_data.directional_light = { kl::normalize(kl::float3 { 0.0f, -1.0f, -1.0f }), 1.0f };
    }

    ps_data.object_color = material->color;
    ps_data.object_material = {
        material->texture_blend,
        material->reflection_factor,
        material->refraction_factor,
        material->refraction_index,
    };
    ps_data.camera_info = { camera.origin, {} };
    shaders.pixel_shader.update_cbuffer(ps_data);

    state->gpu->draw(state->default_meshes["cube"]->graphics_buffer);
    state->gpu->bind_internal_views();
}

void render_selected_texture(editor_state* state, const kl::object<kl::texture>& texture, const kl::int2& viewport_size)
{
    const float min_size = (float) min(viewport_size.x, viewport_size.y);
    ImGui::SetCursorPos(ImVec2 {
        (ImGui::GetWindowWidth() - min_size) * 0.5f,
        (ImGui::GetWindowHeight() - min_size) * 0.5f,
    });
    ImGui::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void show_material_info(editor_state* state, const kl::object<kl::material>& material)
{

}

void show_texture_info(editor_state* state, const kl::object<kl::texture>& texture)
{

}
