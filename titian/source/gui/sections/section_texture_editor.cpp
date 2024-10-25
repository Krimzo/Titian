#include "titian.h"


titian::GUISectionTextureEditor::GUISectionTextureEditor()
    : GUISection("GUISectionTextureEditor")
{}

void titian::GUISectionTextureEditor::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    Scene& scene = Layers::get<GameLayer>().scene();

    Ref<Texture> texture;
    if (scene.textures.contains(selected_texture)) {
        texture = scene.textures.at(selected_texture);
    }

    if (im::Begin("Texture Editor")) {
        const float available_width = im::GetContentRegionAvail().x;
        im::Columns(2, "TextureEditorColumns", false);

        im::SetColumnWidth(im::GetColumnIndex(), available_width * 0.25f);
        if (im::BeginChild("Textures")) {
            display_textures(scene);
        }
        im::EndChild();
        im::NextColumn();

        im::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        im::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        im::PushStyleColor(ImGuiCol_Border, ImVec4{ 1.0f, 1.0f, 1.0f, 0.5f });

        if (im::BeginChild("Texture View", {})) {
            if (texture) {
                render_selected_texture(*texture);
            }
        }
        im::EndChild();

        if (auto file = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            if (classify_file(file.value()) == FileType::TEXTURE) {
                const String name = fs::path(file.value()).filename().string();
                Texture* texture = scene.helper_new_texture(Scene::generate_unique_name(name, scene.textures));
                texture->image.load_from_file(file.value());
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

void titian::GUISectionTextureEditor::display_textures(Scene& scene)
{
    if (im::BeginPopupContextWindow("NewTexture", ImGuiPopupFlags_MouseButtonMiddle)) {
        im::Text("New Texture");

        if (auto opt_name = gui_input_waited("##CreateTextureInput", {})) {
            const auto& name = opt_name.value();
            if (!name.empty() && !scene.textures.contains(name)) {
                Ref texture = new Texture();
                scene.textures[name] = texture;
                im::CloseCurrentPopup();
            }
        }
        im::EndPopup();
    }
    const String filter = gui_input_continuous("Search###TextureEditor");
    for (const auto& [texture_name, texture] : scene.textures) {
        if (!filter.empty() && texture_name.find(filter) == -1) {
            continue;
        }

        if (im::Selectable(texture_name.data(), texture_name == selected_texture)) {
            selected_texture = texture_name;
        }

        if (im::BeginPopupContextItem(texture_name.data(), ImGuiPopupFlags_MouseButtonRight)) {
            bool should_break = false;
            im::Text("Edit Texture");

            if (auto opt_name = gui_input_waited("##RenameTextureInput", texture_name)) {
                const auto& new_name = opt_name.value();
                if (!new_name.empty() && !scene.textures.contains(new_name)) {
                    if (selected_texture == texture_name) {
                        selected_texture = new_name;
                    }
                    for (auto& [_, material] : scene.materials) {
                        if (material->color_texture_name == texture_name) material->color_texture_name = new_name;
                        if (material->normal_texture_name == texture_name) material->normal_texture_name = new_name;
                        if (material->roughness_texture_name == texture_name) material->roughness_texture_name = new_name;
                    }
                    for (auto& [_, entity] : scene.entities()) {
                        if (Camera* camera = &entity.as<Camera>()) {
                            if (camera->skybox_texture_name == texture_name) camera->skybox_texture_name = new_name;
                            if (camera->target_texture_name == texture_name) camera->target_texture_name = new_name;
                        }
                    }
                    scene.textures[new_name] = texture;
                    scene.textures.erase(texture_name);
                    should_break = true;
                    im::CloseCurrentPopup();
                }
            }

            if (im::Button("Delete", { -1.0f, 0.0f })) {
                if (selected_texture == texture_name) {
                    selected_texture = "/";
                }
                scene.textures.erase(texture_name);
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

void titian::GUISectionTextureEditor::render_selected_texture(Texture& texture)
{
    const ImVec2 region_size = im::GetContentRegionAvail();
    const Int2 image_res = texture.resolution();
    ImVec2 image_scale{ (float) image_res.x, (float) image_res.y };
    image_scale *= kl::min(region_size.x / image_scale.x, region_size.y / image_scale.y);
    im::SetCursorPos(im::GetCursorPos() + (region_size - image_scale) * 0.5f);
    im::Image(texture.shader_view.get(), image_scale);
}

void titian::GUISectionTextureEditor::show_texture_properties(Texture* texture)
{
	GUILayer& gui_layer = Layers::get<GUILayer>();

    if (im::Begin("Texture Properties") && texture) {
        im::Text("Info");

        im::Text("Name: ");
        im::SameLine();
        gui_colored_text(selected_texture, gui_layer.special_color);

        Int2 size = texture->resolution();
        im::DragInt2("Size", &size.x, 0.0f);

        int pixel_count = size.x * size.y;
        im::DragInt("Pixel Count", &pixel_count, 0.0f);

        bool cube_map = texture->is_cube();
        if (im::Checkbox("Cube Map", &cube_map)) {
            if (cube_map) {
                if (texture->reload_as_cube()) {
                    texture->create_shader_view(nullptr);
                }
            }
            else {
                texture->reload_as_2D();
                texture->create_shader_view(nullptr);
            }
        }
    }
    im::End();
}
