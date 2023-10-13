export module gui_explorer;

export import gui_render;

enum class FileType
{
    OTHER = 0,
    CODE,
    SCRIPT,
    MESH,
    TEXTURE,
    SCENE,
};

void handle_file_entry(EditorState* state, const std::filesystem::path& file);
void handle_directory_entry(EditorState* state, const std::filesystem::path& directory, bool is_parent_dir);
FileType classify_file(const std::filesystem::path& file);
kl::dx::ShaderView file_icon(EditorState* state, FileType type);
void drag_file(const std::filesystem::path& file, FileType type, const kl::dx::ShaderView& texture);

export void gui_explorer(EditorState* state)
{
    std::list<std::filesystem::path> directories = {};
    std::list<std::filesystem::path> files = {};
    
    try {
        for (auto& entry : std::filesystem::directory_iterator(state->gui_state->explorer_path)) {
            (entry.is_directory() ? directories : files).push_back(entry);
        }
    }
    catch (std::exception ignored) {
    }

    if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        const float window_width = ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.0f;
        const float column_count = window_width / (state->gui_state->explorer_icon_size + ImGui::GetStyle().CellPadding.x * 2.0f);

        ImGui::Text(state->gui_state->explorer_path.c_str());
        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2 { 0.0f, 4.0f });

        if (ImGui::BeginTable("##ExplorerTable", (int) column_count)) {
            const std::filesystem::path current_path = { state->gui_state->explorer_path };
            if (current_path.has_parent_path()) {
                ImGui::TableNextColumn();
                handle_directory_entry(state, current_path.parent_path(), true);
            }

            for (auto& dir : directories) {
                ImGui::TableNextColumn();
                handle_directory_entry(state, dir, false);
            }

            for (auto& file : files) {
                ImGui::TableNextColumn();
                handle_file_entry(state, file);
            }

            ImGui::EndTable();
        }

        ImGui::PopStyleVar();

        if (ImGui::BeginPopupContextWindow()) {
            ImGui::SliderInt("Icon Size", &state->gui_state->explorer_icon_size, 25, 250);
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void handle_file_entry(EditorState* state, const std::filesystem::path& file)
{
    const std::string path = std::filesystem::absolute(file).string();
    const FileType file_type = classify_file(file);
    const kl::dx::ShaderView icon = file_icon(state, file_type);

    constexpr float padding = 5.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const float icon_size = (float) state->gui_state->explorer_icon_size;
    const float text_height = ImGui::CalcTextSize(path.c_str()).y;

    if (ImGui::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = ImGui::GetCursorPos();
        if (ImGui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
            ShellExecuteA(nullptr, nullptr, path.c_str(), nullptr, nullptr, 5);
        }
        drag_file(path, file_type, icon);

        ImGui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        ImGui::Text(file.filename().string().c_str());
    }
    ImGui::EndChild();

    ImGui::PopStyleVar(2);
}

void handle_directory_entry(EditorState* state, const std::filesystem::path& directory, const bool is_parent_dir)
{
    const std::string path = std::filesystem::absolute(directory).string();
    kl::dx::ShaderView icon = state->gui_state->textures.folder->shader_view;
    std::error_code ignored_error = {};
    if (is_parent_dir) {
        icon = state->gui_state->textures.folder_parent->shader_view;
    }
    else if (std::filesystem::is_empty(directory, ignored_error)) {
        icon = state->gui_state->textures.folder_empty->shader_view;
    }

    constexpr float padding = 5.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0.0f, 0.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { padding, padding });

    const float icon_size = (float) state->gui_state->explorer_icon_size;
    const float text_height = ImGui::CalcTextSize(path.c_str()).y;

    constexpr float shadow_size = 2.0f;
    const ImVec2 child_size = { icon_size + padding * 2, icon_size + text_height + padding * 4.0f };

    if (ImGui::BeginChild(kl::format(path, "_elder").c_str(), { child_size.x + shadow_size, child_size.y + shadow_size })) {
        if (ImGui::BeginChild(path.c_str(), child_size, true, ImGuiWindowFlags_NoScrollbar)) {
            const ImVec2 cursor_pos = ImGui::GetCursorPos();
            if (ImGui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
                state->gui_state->explorer_path = path;
            }

            ImGui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
            ImGui::Text(directory.filename().string().c_str());
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::PopStyleVar(2);
}

FileType classify_file(const std::filesystem::path& file)
{
    const std::string extension = file.extension().string();
    if (extension == ".h" || extension == ".cpp") {
        return FileType::CODE;
    }
    if (extension == ".dll" || extension == ".class") {
        return FileType::SCRIPT;
    }
    if (extension == ".obj") {
        return FileType::MESH;
    }
    if (extension == ".jpg" || extension == ".png") {
        return FileType::TEXTURE;
    }
    if (extension == ".titian") {
        return FileType::SCENE;
    }
    return FileType::OTHER;
}

kl::dx::ShaderView file_icon(EditorState* state, const FileType type)
{
    switch (type) {
    case FileType::CODE:
        return state->gui_state->textures.code->shader_view;
    case FileType::SCRIPT:
        return state->gui_state->textures.script->shader_view;
    case FileType::MESH:
        return state->gui_state->textures.mesh->shader_view;
    case FileType::TEXTURE:
        return state->gui_state->textures.texture->shader_view;
    case FileType::SCENE:
        return state->gui_state->textures.scene->shader_view;
    }
    return state->gui_state->textures.file->shader_view;
}

void drag_file(const std::filesystem::path& file, const FileType type, const kl::dx::ShaderView& texture)
{
    const std::string path = file.string();
    switch (type) {
    case FileType::CODE:
        gui::drag_drop::write_data("CodeFile", path, texture);
        break;
    case FileType::SCRIPT:
        gui::drag_drop::write_data("ScriptFile", path, texture);
        break;
    case FileType::MESH:
        gui::drag_drop::write_data("MeshFile", path, texture);
        break;
    case FileType::TEXTURE:
        gui::drag_drop::write_data("TextureFile", path, texture);
        break;
    case FileType::SCENE:
        gui::drag_drop::write_data("SceneFile", path, texture);
        break;
    }
}
