#include "main.h"


titian::GUISectionTextureEditor::GUISectionTextureEditor(const LayerPackage& package)
    : GUISection("GUISectionTextureEditor", package)
{}

void titian::GUISectionTextureEditor::render_gui()
{
    const TimeBomb _ = this->time_it();

    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;
    kl::Object texture = scene->get_texture(this->selected_texture);

    if (imgui::Begin("Texture Editor")) {
        const float available_width = imgui::GetContentRegionAvail().x;
        imgui::Columns(2, "TextureEditorColumns", false);

        imgui::SetColumnWidth(imgui::GetColumnIndex(), available_width * 0.25f);
        if (imgui::BeginChild("Textures")) {
            display_textures(gpu, scene);
        }
        imgui::EndChild();
        imgui::NextColumn();

        imgui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        imgui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        imgui::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (imgui::BeginChild("Texture View", {})) {
            const kl::Int2 viewport_size = { (int) imgui::GetContentRegionAvail().x, (int) imgui::GetContentRegionAvail().y };
            if (texture) {
                render_selected_texture(&texture, viewport_size);
            }
        }
        imgui::EndChild();

        if (const std::optional file = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::TEXTURE) {
                const std::string name = std::filesystem::path(file.value()).filename().string();
                Texture* texture = scene->helper_new_texture(scene->generate_unique_name(name, scene->textures));
                texture->data_buffer.load_from_file(file.value());
                texture->reload_as_2D();
                texture->create_shader_view();
            }
        }

        imgui::PopStyleColor();
        imgui::PopStyleVar(2);

        if (texture) {
            show_texture_properties(&texture);
        }
    }
    imgui::End();
}

void titian::GUISectionTextureEditor::display_textures(kl::GPU* gpu, Scene* scene)
{
    // New texture
    if (imgui::BeginPopupContextWindow("NewTexture", ImGuiPopupFlags_MouseButtonMiddle)) {
        imgui::Text("New Texture");

        if (std::optional opt_name = gui_input_waited("##CreateTextureInput", {})) {
            const std::string& name = opt_name.value();
            if (!name.empty() && !scene->textures.contains(name)) {
                kl::Object texture = new Texture(gpu);
                scene->textures[name] = texture;
                imgui::CloseCurrentPopup();
            }
        }
        imgui::EndPopup();
    }

    // Textures
    const std::string filter = gui_input_continuous("Search###TextureEditor");
    for (const auto& [texture_name, texture] : scene->textures) {
        if (!filter.empty() && texture_name.find(filter) == -1) {
            continue;
        }

        if (imgui::Selectable(texture_name.c_str(), texture_name == this->selected_texture)) {
            this->selected_texture = texture_name;
        }

        if (imgui::BeginPopupContextItem(texture_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            imgui::Text("Edit Texture");

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
                    imgui::CloseCurrentPopup();
                }
            }

            if (imgui::Button("Delete", { -1.0f, 0.0f })) {
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
                imgui::CloseCurrentPopup();
            }

            imgui::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionTextureEditor::render_selected_texture(Texture* texture, const kl::Int2 viewport_size)
{
    const float min_size = static_cast<float>(std::min(viewport_size.x, viewport_size.y));
    imgui::SetCursorPos(ImVec2{
        (imgui::GetWindowWidth() - min_size) * 0.5f,
        (imgui::GetWindowHeight() - min_size) * 0.5f,
    });
    imgui::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void titian::GUISectionTextureEditor::show_texture_properties(Texture* texture)
{
    if (imgui::Begin("Texture Properties") && texture) {
        imgui::Text("Info");

        imgui::Text("Name: ");
        imgui::SameLine();
        gui_colored_text(selected_texture, gui_layer->special_color);

        kl::Int2 size = texture->data_buffer.size();
        imgui::DragInt2("Size", size, 0.0f);

        int pixel_count = texture->data_buffer.pixel_count();
        imgui::DragInt("Pixel Count", &pixel_count, 0.0f);

        bool cube_map = texture->is_cube();
        if (imgui::Checkbox("Cube Map", &cube_map)) {
            if (cube_map) {
                if (texture->reload_as_cube()) {
                    texture->create_shader_view(nullptr);
                }
            }
            else {
                texture->reload_as_2D(false, false);
                texture->create_shader_view(nullptr);
            }
        }
    }
    imgui::End();
}
