#include "titian.h"


titian::GUISectionExplorer::GUISectionExplorer()
    : GUISection("GUISectionExplorer")
{
    AppLayer* app_layer = Layers::get<AppLayer>();

    auto create_texture = [&](Ref<Texture>& texture, const char* filename)
    {
        texture = new Texture(&app_layer->gpu);
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D(false, false);
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(default_file_texture, "package/textures/default_file.png"); });
    queue.add_task([&] { create_texture(mesh_file_texture, "package/textures/mesh_file.png"); });
    queue.add_task([&] { create_texture(texture_file_texture, "package/textures/texture_file.png"); });
    queue.add_task([&] { create_texture(script_file_texture, "package/textures/script_file.png"); });
    queue.add_task([&] { create_texture(shader_file_texture, "package/textures/shader_file.png"); });
    queue.add_task([&] { create_texture(scene_file_texture, "package/textures/scene_file.png"); });
    queue.add_task([&] { create_texture(default_dir_texture, "package/textures/default_dir.png"); });
    queue.add_task([&] { create_texture(parent_dir_texture, "package/textures/parent_dir.png"); });
    queue.finalize();
}

void titian::GUISectionExplorer::render_gui()
{
    const TimeBomb _ = this->time_it();

    List<fs::path> directories{};
    List<fs::path> files{};

    try {
        for (auto& entry : fs::directory_iterator(m_path)) {
            (entry.is_directory() ? directories : files).push_back(entry);
        }
    }
    catch (Exception) {
    }

    if (im::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        // New file
        if (im::BeginPopupContextWindow("NewFile", ImGuiPopupFlags_MouseButtonMiddle)) {
            im::Text("Create");
            const String name_input = gui_input_continuous("##CreateFileInput");
            if (!name_input.empty()) {
                if (im::MenuItem("Directory")) {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    const String full_path = stream.str();
                    if (!fs::exists(full_path)) {
                        fs::create_directory(full_path);
                        im::CloseCurrentPopup();
                    }
                }
                if (im::MenuItem("Text File")) {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    const String full_path = stream.str();
                    if (!fs::exists(full_path)) {
                        std::ofstream _{ full_path };
                        im::CloseCurrentPopup();
                    }
                }
                if (im::MenuItem("Script File")) {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    if (name_input.find(FILE_EXTENSION_CHAI) == -1) {
                        stream << FILE_EXTENSION_CHAI;
                    }
                    const String full_path = stream.str();
                    if (!fs::exists(full_path)) {
                        std::ofstream file{ full_path };
                        file << get_default_script();
                        im::CloseCurrentPopup();
                    }
                }
                if (im::MenuItem("Material Shader File")) {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    if (name_input.find(FILE_EXTENSION_HLSL) == -1) {
                        stream << FILE_EXTENSION_HLSL;
                    }
                    const String full_path = stream.str();
                    if (!fs::exists(full_path)) {
                        std::ofstream file{ full_path };
                        file << get_default_material_shader();
                        im::CloseCurrentPopup();
                    }
                }
                if (im::MenuItem("Camera Shader File")) {
                    StringStream stream{};
                    stream << m_path << "/" << name_input;
                    if (name_input.find(FILE_EXTENSION_HLSL) == -1) {
                        stream << FILE_EXTENSION_HLSL;
                    }
                    const String full_path = stream.str();
                    if (!fs::exists(full_path)) {
                        std::ofstream file{ full_path };
                        file << get_default_camera_shader();
                        im::CloseCurrentPopup();
                    }
                }
            }
            im::EndPopup();
        }

        GUILayer* gui_layer = Layers::get<GUILayer>();
        const float icon_size = m_icon_size * gui_layer->dpi_scaling;
        const float window_width = im::GetWindowWidth() - im::GetStyle().WindowPadding.x * 2.0f;
        const float icon_width = icon_size + im::GetStyle().CellPadding.x * 2.0f;
        const int column_count = kl::max((int) (window_width / icon_width), 1);

        im::Text(m_path.c_str());
        im::Separator();

        im::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 4.0f });

        if (im::BeginTable("##ExplorerTable", column_count)) {
            const fs::path current_path = { m_path };
            if (current_path.has_parent_path()) {
                im::TableNextColumn();
                handle_directory_entry(current_path.parent_path(), true);
            }
            for (auto& dir : directories) {
                im::TableNextColumn();
                handle_directory_entry(dir, false);
            }
            for (auto& file : files) {
                im::TableNextColumn();
                handle_file_entry(file);
            }
            im::EndTable();
        }

        im::PopStyleVar();

        if (im::IsKeyDown(ImGuiKey_LeftShift) && im::BeginPopupContextWindow()) {
            im::SliderFloat("Icon Size", &m_icon_size, 25.0f, 250.0f);
            im::EndPopup();
        }
    }
    im::End();
}

void titian::GUISectionExplorer::handle_file_entry(const fs::path& file)
{
    const String path = fs::absolute(file).string();
    const FileType file_type = classify_file(file);
    const dx::ShaderView icon = file_icon(file_type);

    constexpr float padding = 5.0f;
    im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const float icon_size = m_icon_size * Layers::get<GUILayer>()->dpi_scaling;
    const float text_height = im::CalcTextSize(path.c_str()).y;

    if (im::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = im::GetCursorPos();
        if (im::ImageButton(path.c_str(), icon.get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
            ShellExecuteA(nullptr, nullptr, path.c_str(), nullptr, nullptr, 5);
        }
        gui_set_drag_drop<String>(DRAG_FILE_ID, path, icon);

        im::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        im::Text(file.filename().string().c_str());
    }
    im::EndChild();

    im::PopStyleVar(2);

    if (im::BeginPopupContextItem(file.string().c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if (Optional opt_name = gui_input_waited("##RenameFileInput", file.filename().string())) {
            const String& name = opt_name.value();
            if (!name.empty()) {
                fs::path new_file = file;
                new_file.replace_filename(opt_name.value());

				if (!fs::exists(new_file)) {
                    fs::rename(file, new_file);
                    Logger::log("Renamed file ", format_path(file), " to ", format_path(new_file));
					im::CloseCurrentPopup();
				}
                else {
                    Logger::log("Failed to rename file ", format_path(file), " to ", format_path(new_file));
                }
            }
        }
        if (im::Button("Delete", { -1.0f, 0.0f })) {
            Logger::log("Deleted file ", format_path(file));
            fs::remove(file);
            im::CloseCurrentPopup();
        }
        else {
            Error ignored{};
            im::Text(format_byte_size(fs::file_size(file, ignored)).c_str());
        }
        im::EndPopup();
    }
}

void titian::GUISectionExplorer::handle_directory_entry(const fs::path& dir, const bool is_parent_dir)
{
    const String path = fs::absolute(dir).string();
    const dx::ShaderView icon = is_parent_dir ? parent_dir_texture->shader_view : default_dir_texture->shader_view;

    constexpr float padding = 5.0f;
    im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const float icon_size = m_icon_size * Layers::get<GUILayer>()->dpi_scaling;
    const float text_height = im::CalcTextSize(path.c_str()).y;

    if (im::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = im::GetCursorPos();
        if (im::ImageButton(path.c_str(), icon.get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
            this->m_path = path;
        }

        if (Optional dragged_path = gui_get_drag_drop<String>(DRAG_FILE_ID)) {
            handle_item_transfer(dragged_path.value(), path);
        }
        if (Optional dragged_path = gui_get_drag_drop<String>(DRAG_DIR_ID)) {
            handle_item_transfer(dragged_path.value(), path);
        }
        if (!is_parent_dir) {
            gui_set_drag_drop<String>(DRAG_DIR_ID, path, icon);
        }

        im::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        im::Text(dir.filename().string().c_str());
    }
    im::EndChild();

    im::PopStyleVar(2);

    if (!is_parent_dir && im::BeginPopupContextItem(dir.string().c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if (Optional opt_name = gui_input_waited("##RenameDirInput", dir.filename().string())) {
            const String& name = opt_name.value();
            if (!name.empty()) {
                fs::path new_dir = dir;
                new_dir.replace_filename(opt_name.value());

                if (!fs::exists(new_dir)) {
                    fs::rename(dir, new_dir);
                    Logger::log("Renamed directory ", format_path(dir), " to ", format_path(new_dir));
                    im::CloseCurrentPopup();
                }
                else {
                    Logger::log("Failed to rename directory ", format_path(dir), " to ", format_path(new_dir));
                }
            }
        }
        if (im::Button("Delete", { -1.0f, 0.0f })) {
            Logger::log("Deleted directory ", format_path(dir));
			fs::remove_all(dir);
            im::CloseCurrentPopup();
        }
        im::EndPopup();
    }
}

void titian::GUISectionExplorer::handle_item_transfer(const String& item, const String& destination)
{
	const auto item_absolute = fs::absolute(item);
	const auto destination_absolute = fs::absolute(destination);
    if (item_absolute == destination_absolute) {
		Logger::log("Failed to transfer file ", format_path(item_absolute), " to ", format_path(destination_absolute));
        return;
    }

	const String new_item_absolute = destination_absolute.string() + "/" + item_absolute.filename().string();
	if (fs::exists(new_item_absolute)) {
		Logger::log("Failed to transfer file ", format_path(item_absolute), " to ", format_path(new_item_absolute));
		return;
	}

	fs::rename(item_absolute, new_item_absolute);
	Logger::log("Transferred file ", format_path(item_absolute), " to ", format_path(new_item_absolute));
}

dx::ShaderView titian::GUISectionExplorer::file_icon(const FileType type)
{
    switch (type) {
    case FileType::MESH:
        return mesh_file_texture->shader_view;
    case FileType::TEXTURE:
        return texture_file_texture->shader_view;
    case FileType::SCRIPT:
        return script_file_texture->shader_view;
    case FileType::SHADER:
        return shader_file_texture->shader_view;
    case FileType::SCENE:
        return scene_file_texture->shader_view;
    }
    return default_file_texture->shader_view;
}
