#include "main.h"


titian::GUISectionTextureEditor::GUISectionTextureEditor(EditorLayer* editor_layer, GUILayer* gui_layer)
{
    this->editor_layer = editor_layer;
    this->gui_layer = gui_layer;
}

void titian::GUISectionTextureEditor::render_gui()
{
    kl::GPU* gpu = &editor_layer->game_layer->app_layer->gpu;
    Scene* scene = &editor_layer->game_layer->scene;
    Texture* texture = &scene->get_texture(this->selected_texture);

    if (ImGui::Begin("Texture Editor")) {
        const float available_width = ImGui::GetContentRegionAvail().x;
        ImGui::Columns(2, "TextureEditorColumns", false);

        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), available_width * 0.25f);
        if (ImGui::BeginChild("Textures")) {
            display_textures(gpu, scene);
        }
        ImGui::EndChild();
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (ImGui::BeginChild("Texture View", {})) {
            const kl::Int2 viewport_size = { (int) ImGui::GetContentRegionAvail().x, (int) ImGui::GetContentRegionAvail().y };
            if (texture) {
                render_selected_texture(texture, viewport_size);
            }
        }
        ImGui::EndChild();

        if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
            if (texture && classify_file(file.value()) == FileType::TEXTURE) {
                const std::filesystem::path path = file.value();
                texture->data_buffer.load_from_file(path.string());
                texture->load_as_2D(false, false);
                texture->create_shader_view(nullptr);
            }
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        if (texture) {
            show_texture_properties(texture);
        }
    }
    ImGui::End();
}

void titian::GUISectionTextureEditor::display_textures(kl::GPU* gpu, Scene* scene)
{
    // New texture
    if (ImGui::BeginPopupContextWindow("NewTexture", ImGuiPopupFlags_MouseButtonMiddle)) {
        ImGui::Text("New Texture");

        if (std::optional opt_name = gui_input_waited("##CreateTextureInput", {})) {
            const std::string& name = opt_name.value();
            if (!name.empty() && !scene->textures.contains(name)) {
                kl::Object texture = new Texture(gpu);
                scene->textures[name] = texture;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    // Textures
    const std::string filter = gui_input_continuous("Search###TextureEditor");
    for (const auto& [texture_name, texture] : scene->textures) {
        if (!filter.empty() && texture_name.find(filter) == -1) {
            continue;
        }

        if (ImGui::Selectable(texture_name.c_str(), texture_name == this->selected_texture)) {
            this->selected_texture = texture_name;
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

void titian::GUISectionTextureEditor::render_selected_texture(Texture* texture, const kl::Int2 viewport_size)
{
    const float min_size = static_cast<float>(std::min(viewport_size.x, viewport_size.y));
    ImGui::SetCursorPos(ImVec2{
        (ImGui::GetWindowWidth() - min_size) * 0.5f,
        (ImGui::GetWindowHeight() - min_size) * 0.5f,
    });
    ImGui::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void titian::GUISectionTextureEditor::show_texture_properties(Texture* texture)
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
