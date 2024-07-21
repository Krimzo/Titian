#include "titian.h"


titian::GUISectionTextureEditor::GUISectionTextureEditor(const LayerPackage& package)
    : GUISection("GUISectionTextureEditor", package)
{}

void titian::GUISectionTextureEditor::render_gui()
{
    const TimeBomb _ = this->time_it();

    kl::GPU* gpu = &app_layer->gpu;
    Scene* scene = &game_layer->scene;
    Ref texture = scene->get_texture(this->selected_texture);

    if (im::Begin("Texture Editor")) {
        const float available_width = im::GetContentRegionAvail().x;
        im::Columns(2, "TextureEditorColumns", false);

        im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
        if (im::BeginChild("Textures")) {
            display_textures(gpu, scene);
        }
        im::EndChild();
        im::NextColumn();

        im::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        im::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        im::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (im::BeginChild("Texture View", {})) {
            const Int2 viewport_size = { (int) im::GetContentRegionAvail().x, (int) im::GetContentRegionAvail().y };
            if (texture) {
                render_selected_texture(&texture, viewport_size);
            }
        }
        im::EndChild();

        if (const Optional file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::TEXTURE) {
                const String name = fs::path(file.value()).filename().string();
                Texture* texture = scene->helper_new_texture(scene->generate_unique_name(name, scene->textures));
                texture->data_buffer.load_from_file(file.value());
                texture->reload_as_2D();
                texture->create_shader_view();
            }
        }

        im::PopStyleColor();
        im::PopStyleVar(2);

        if (texture) {
            show_texture_properties(&texture);
        }
    }
    im::End();
}

void titian::GUISectionTextureEditor::display_textures(kl::GPU* gpu, Scene* scene)
{
    // New texture
    if (im::BeginPopupContextWindow("NewTexture", ImGuiPopupFlags_MouseButtonMiddle)) {
        im::Text("New Texture");

        if (Optional opt_name = gui_input_waited("##CreateTextureInput", {})) {
            const String& name = opt_name.value();
            if (!name.empty() && !scene->textures.contains(name)) {
                Ref texture = new Texture(gpu);
                scene->textures[name] = texture;
                im::CloseCurrentPopup();
            }
        }
        im::EndPopup();
    }

    // Textures
    const String filter = gui_input_continuous("Search###TextureEditor");
    for (const auto& [texture_name, texture] : scene->textures) {
        if (!filter.empty() && texture_name.find(filter) == -1) {
            continue;
        }

        if (im::Selectable(texture_name.c_str(), texture_name == this->selected_texture)) {
            this->selected_texture = texture_name;
        }

        if (im::BeginPopupContextItem(texture_name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            im::Text("Edit Texture");

            if (Optional opt_name = gui_input_waited("##RenameTextureInput", texture_name)) {
                const String& name = opt_name.value();
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
                    im::CloseCurrentPopup();
                }
            }

            if (im::Button("Delete", { -1.0f, 0.0f })) {
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
                im::CloseCurrentPopup();
            }

            im::EndPopup();
            if (should_break) {
                break;
            }
        }
    }
}

void titian::GUISectionTextureEditor::render_selected_texture(Texture* texture, const Int2 viewport_size)
{
    const float min_size = static_cast<float>(std::min(viewport_size.x, viewport_size.y));
    im::SetCursorPos(ImVec2{
        (im::GetWindowWidth() - min_size) * 0.5f,
        (im::GetWindowHeight() - min_size) * 0.5f,
    });
    im::Image(texture->shader_view.Get(), { min_size, min_size }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
}

void titian::GUISectionTextureEditor::show_texture_properties(Texture* texture)
{
    if (im::Begin("Texture Properties") && texture) {
        im::Text("Info");

        im::Text("Name: ");
        im::SameLine();
        gui_colored_text(selected_texture, gui_layer->special_color);

        Int2 size = texture->data_buffer.size();
        im::DragInt2("Size", size, 0.0f);

        int pixel_count = texture->data_buffer.pixel_count();
        im::DragInt("Pixel Count", &pixel_count, 0.0f);

        bool cube_map = texture->is_cube();
        if (im::Checkbox("Cube Map", &cube_map)) {
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
    im::End();
}
