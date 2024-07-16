#include "main.h"


titian::GUISectionExplorer::GUISectionExplorer(const LayerPackage& package)
    : GUISection("GUISectionExplorer", package)
{
    auto create_texture = [&](kl::Object<Texture>& texture, const char* filename)
    {
        texture = new Texture(&app_layer->gpu);
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D(false, false);
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(default_file_texture, "builtin/textures/default_file.png"); });
    queue.add_task([&] { create_texture(mesh_file_texture, "builtin/textures/mesh_file.png"); });
    queue.add_task([&] { create_texture(texture_file_texture, "builtin/textures/texture_file.png"); });
    queue.add_task([&] { create_texture(script_file_texture, "builtin/textures/script_file.png"); });
    queue.add_task([&] { create_texture(shader_file_texture, "builtin/textures/shader_file.png"); });
    queue.add_task([&] { create_texture(scene_file_texture, "builtin/textures/scene_file.png"); });
    queue.add_task([&] { create_texture(default_dir_texture, "builtin/textures/default_dir.png"); });
    queue.add_task([&] { create_texture(parent_dir_texture, "builtin/textures/parent_dir.png"); });
    queue.finalize();
}

void titian::GUISectionExplorer::render_gui()
{
    const TimeBomb _ = this->time_it();

    std::list<std::filesystem::path> directories{};
    std::list<std::filesystem::path> files{};

    try {
        for (auto& entry : std::filesystem::directory_iterator(m_path)) {
            (entry.is_directory() ? directories : files).push_back(entry);
        }
    }
    catch (std::exception) {
    }

    if (imgui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        // New file
        if (imgui::BeginPopupContextWindow("NewFile", ImGuiPopupFlags_MouseButtonMiddle)) {
            imgui::Text("Create");
            const std::string name_input = gui_input_continuous("##CreateFileInput");
            if (!name_input.empty()) {
                if (imgui::MenuItem("Directory")) {
                    std::stringstream stream{};
                    stream << m_path << "/" << name_input;
                    const std::string full_path = stream.str();
                    if (!std::filesystem::exists(full_path)) {
                        std::filesystem::create_directory(full_path);
                        imgui::CloseCurrentPopup();
                    }
                }
                if (imgui::MenuItem("Text File")) {
                    std::stringstream stream{};
                    stream << m_path << "/" << name_input;
                    const std::string full_path = stream.str();
                    if (!std::filesystem::exists(full_path)) {
                        std::ofstream _{ full_path };
                        imgui::CloseCurrentPopup();
                    }
                }
                if (imgui::MenuItem("Script File")) {
                    std::stringstream stream{};
                    stream << m_path << "/" << name_input;
                    if (name_input.find(FILE_EXTENSION_INTERP_SCRIPT) == -1) {
                        stream << FILE_EXTENSION_INTERP_SCRIPT;
                    }
                    const std::string full_path = stream.str();
                    if (!std::filesystem::exists(full_path)) {
                        std::ofstream file{ full_path };
                        file << get_default_script();
                        imgui::CloseCurrentPopup();
                    }
                }
                if (imgui::MenuItem("Material Shader File")) {
                    std::stringstream stream{};
                    stream << m_path << "/" << name_input;
                    if (name_input.find(FILE_EXTENSION_SHADER) == -1) {
                        stream << FILE_EXTENSION_SHADER;
                    }
                    const std::string full_path = stream.str();
                    if (!std::filesystem::exists(full_path)) {
                        std::ofstream file{ full_path };
                        file << get_default_material_shader();
                        imgui::CloseCurrentPopup();
                    }
                }
                if (imgui::MenuItem("Camera Shader File")) {
                    std::stringstream stream{};
                    stream << m_path << "/" << name_input;
                    if (name_input.find(FILE_EXTENSION_SHADER) == -1) {
                        stream << FILE_EXTENSION_SHADER;
                    }
                    const std::string full_path = stream.str();
                    if (!std::filesystem::exists(full_path)) {
                        std::ofstream file{ full_path };
                        file << get_default_camera_shader();
                        imgui::CloseCurrentPopup();
                    }
                }
            }
            imgui::EndPopup();
        }

        const float icon_size = m_icon_size * gui_layer->dpi_scaling;
        const float window_width = imgui::GetWindowWidth() - imgui::GetStyle().WindowPadding.x * 2.0f;
        const float icon_width = icon_size + imgui::GetStyle().CellPadding.x * 2.0f;
        const int column_count = std::max((int) (window_width / icon_width), 1);

        imgui::Text(m_path.c_str());
        imgui::Separator();

        imgui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 4.0f });

        if (imgui::BeginTable("##ExplorerTable", column_count)) {
            const std::filesystem::path current_path = { m_path };
            if (current_path.has_parent_path()) {
                imgui::TableNextColumn();
                handle_directory_entry(current_path.parent_path(), true);
            }
            for (auto& dir : directories) {
                imgui::TableNextColumn();
                handle_directory_entry(dir, false);
            }
            for (auto& file : files) {
                imgui::TableNextColumn();
                handle_file_entry(file);
            }
            imgui::EndTable();
        }

        imgui::PopStyleVar();

        if (imgui::IsKeyDown(ImGuiKey_LeftShift) && imgui::BeginPopupContextWindow()) {
            imgui::SliderFloat("Icon Size", &m_icon_size, 25.0f, 250.0f);
            imgui::EndPopup();
        }
    }
    imgui::End();
}

void titian::GUISectionExplorer::handle_file_entry(const std::filesystem::path& file)
{
    const std::string path = std::filesystem::absolute(file).string();
    const FileType file_type = classify_file(file);
    const kl::dx::ShaderView icon = file_icon(file_type);

    constexpr float padding = 5.0f;
    imgui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const float icon_size = m_icon_size * gui_layer->dpi_scaling;
    const float text_height = imgui::CalcTextSize(path.c_str()).y;

    if (imgui::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = imgui::GetCursorPos();
        if (imgui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
            ShellExecuteA(nullptr, nullptr, path.c_str(), nullptr, nullptr, 5);
        }
        gui_set_drag_drop<std::string>(DRAG_FILE_ID, path, icon);

        imgui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        imgui::Text(file.filename().string().c_str());
    }
    imgui::EndChild();

    imgui::PopStyleVar(2);

    if (imgui::BeginPopupContextItem(file.string().c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if (std::optional opt_name = gui_input_waited("##RenameFileInput", file.filename().string())) {
            const std::string& name = opt_name.value();
            if (!name.empty()) {
                std::filesystem::path new_file = file;
                new_file.replace_filename(opt_name.value());

				if (!std::filesystem::exists(new_file)) {
                    std::filesystem::rename(file, new_file);
                    Logger::log("Renamed file ", format_path(file), " to ", format_path(new_file));
					imgui::CloseCurrentPopup();
				}
                else {
                    Logger::log("Failed to rename file ", format_path(file), " to ", format_path(new_file));
                }
            }
        }
        if (imgui::Button("Delete", { -1.0f, 0.0f })) {
            Logger::log("Deleted file ", format_path(file));
            std::filesystem::remove(file);
            imgui::CloseCurrentPopup();
        }
        else {
            std::error_code ignored{};
            imgui::Text(format_byte_size(std::filesystem::file_size(file, ignored)).c_str());
        }
        imgui::EndPopup();
    }
}

void titian::GUISectionExplorer::handle_directory_entry(const std::filesystem::path& dir, const bool is_parent_dir)
{
    const std::string path = std::filesystem::absolute(dir).string();
    const kl::dx::ShaderView icon = is_parent_dir ? parent_dir_texture->shader_view : default_dir_texture->shader_view;

    constexpr float padding = 5.0f;
    imgui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    imgui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const float icon_size = m_icon_size * gui_layer->dpi_scaling;
    const float text_height = imgui::CalcTextSize(path.c_str()).y;

    if (imgui::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = imgui::GetCursorPos();
        if (imgui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
            this->m_path = path;
        }

        if (std::optional dragged_path = gui_get_drag_drop<std::string>(DRAG_FILE_ID)) {
            handle_item_transfer(dragged_path.value(), path);
        }
        if (std::optional dragged_path = gui_get_drag_drop<std::string>(DRAG_DIR_ID)) {
            handle_item_transfer(dragged_path.value(), path);
        }
        if (!is_parent_dir) {
            gui_set_drag_drop<std::string>(DRAG_DIR_ID, path, icon);
        }

        imgui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        imgui::Text(dir.filename().string().c_str());
    }
    imgui::EndChild();

    imgui::PopStyleVar(2);

    if (!is_parent_dir && imgui::BeginPopupContextItem(dir.string().c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        if (std::optional opt_name = gui_input_waited("##RenameDirInput", dir.filename().string())) {
            const std::string& name = opt_name.value();
            if (!name.empty()) {
                std::filesystem::path new_dir = dir;
                new_dir.replace_filename(opt_name.value());

                if (!std::filesystem::exists(new_dir)) {
                    std::filesystem::rename(dir, new_dir);
                    Logger::log("Renamed directory ", format_path(dir), " to ", format_path(new_dir));
                    imgui::CloseCurrentPopup();
                }
                else {
                    Logger::log("Failed to rename directory ", format_path(dir), " to ", format_path(new_dir));
                }
            }
        }
        if (imgui::Button("Delete", { -1.0f, 0.0f })) {
            Logger::log("Deleted directory ", format_path(dir));
			std::filesystem::remove_all(dir);
            imgui::CloseCurrentPopup();
        }
        imgui::EndPopup();
    }
}

void titian::GUISectionExplorer::handle_item_transfer(const std::string& item, const std::string& destination)
{
	const auto item_absolute = std::filesystem::absolute(item);
	const auto destination_absolute = std::filesystem::absolute(destination);
    if (item_absolute == destination_absolute) {
		Logger::log("Failed to transfer file ", format_path(item_absolute), " to ", format_path(destination_absolute));
        return;
    }

	const std::string new_item_absolute = destination_absolute.string() + "/" + item_absolute.filename().string();
	if (std::filesystem::exists(new_item_absolute)) {
		Logger::log("Failed to transfer file ", format_path(item_absolute), " to ", format_path(new_item_absolute));
		return;
	}

	std::filesystem::rename(item_absolute, new_item_absolute);
	Logger::log("Transferred file ", format_path(item_absolute), " to ", format_path(new_item_absolute));
}

kl::dx::ShaderView titian::GUISectionExplorer::file_icon(const FileType type)
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
