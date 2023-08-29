#include "editor.h"


enum class file_type
{
    OTHER = 0,
    CODE,
    SCRIPT,
    MESH,
    TEXTURE,
    SCENE,
};

void handle_file_entry(editor_state* state, const std::filesystem::path& file);
void handle_directory_entry(editor_state* state, const std::filesystem::path& directory, bool is_parent_dir);
file_type classify_file(const std::filesystem::path& file);
kl::dx::shader_view file_icon(editor_state* state, file_type type);
void drag_file(const std::filesystem::path& file, file_type type, const kl::dx::shader_view& texture);

void gui_explorer(editor_state* state)
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

void handle_file_entry(editor_state* state, const std::filesystem::path& file)
{
    const std::string path = std::filesystem::absolute(file).string();
    const file_type file_type = classify_file(file);
    kl::dx::shader_view icon = file_icon(state, file_type);

    const float padding = 5.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ padding, padding });

    const float icon_size = (float) state->gui_state->explorer_icon_size;
    const float text_height = ImGui::CalcTextSize(path.c_str()).y;

    if (ImGui::BeginChild(path.c_str(), { icon_size + padding * 2, icon_size + text_height + padding * 4.0f }, true, ImGuiWindowFlags_NoScrollbar)) {
        const ImVec2 cursor_pos = ImGui::GetCursorPos();
        if (ImGui::ImageButton(path.c_str(), icon.Get(), { icon_size, icon_size }, ImVec2(0, 1), ImVec2(1, 0))) {
            ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
        }
        drag_file(path, file_type, icon);

        ImGui::SetCursorPos({ cursor_pos.x + padding, cursor_pos.y + icon_size + padding * 3.0f });
        ImGui::Text(file.filename().string().c_str());
    }
    ImGui::EndChild();

    ImGui::PopStyleVar(2);
}

void handle_directory_entry(editor_state* state, const std::filesystem::path& directory, const bool is_parent_dir)
{
    const std::string path = std::filesystem::absolute(directory).string();
    kl::dx::shader_view icon = state->gui_state->textures.folder->shader_view;
    std::error_code ignored_error = {};
    if (is_parent_dir) {
        icon = state->gui_state->textures.folder_parent->shader_view;
    }
    else if (std::filesystem::is_empty(directory, ignored_error)) {
        icon = state->gui_state->textures.folder_empty->shader_view;
    }

    const float padding = 5.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0.0f, 0.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { padding, padding });

    const float icon_size = (float) state->gui_state->explorer_icon_size;
    const float text_height = ImGui::CalcTextSize(path.c_str()).y;

    const float shadow_size = 2.0f;
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

file_type classify_file(const std::filesystem::path& file)
{
    const std::string extension = file.extension().string();
    if (extension == ".h" || extension == ".cpp") {
        return file_type::CODE;
    }
    if (extension == ".dll" || extension == ".class") {
        return file_type::SCRIPT;
    }
    if (extension == ".obj") {
        return file_type::MESH;
    }
    if (extension == ".jpg" || extension == ".png") {
        return file_type::TEXTURE;
    }
    if (extension == ".titian") {
        return file_type::SCENE;
    }
    return file_type::OTHER;
}

kl::dx::shader_view file_icon(editor_state* state, file_type type)
{
    switch (type) {
    case file_type::CODE:
        return state->gui_state->textures.code->shader_view;
    case file_type::SCRIPT:
        return state->gui_state->textures.script->shader_view;
    case file_type::MESH:
        return state->gui_state->textures.mesh->shader_view;
    case file_type::TEXTURE:
        return state->gui_state->textures.texture->shader_view;
    case file_type::SCENE:
        return state->gui_state->textures.scene->shader_view;
    }
    return state->gui_state->textures.file->shader_view;
}

void drag_file(const std::filesystem::path& file, const file_type type, const kl::dx::shader_view& texture)
{
    const std::string path = file.string();
    switch (type) {
    case file_type::CODE:
        GUI::drag_drop::write_data("CodeFile", path, texture);
        break;
    case file_type::SCRIPT:
        GUI::drag_drop::write_data("ScriptFile", path, texture);
        break;
    case file_type::MESH:
        GUI::drag_drop::write_data("MeshFile", path, texture);
        break;
    case file_type::TEXTURE:
        GUI::drag_drop::write_data("TextureFile", path, texture);
        break;
    case file_type::SCENE:
        GUI::drag_drop::write_data("SceneFile", path, texture);
        break;
    }
}
