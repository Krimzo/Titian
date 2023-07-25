#include "gui/gui_render.h"


enum class file_type
{
    other = 0,
    code,
    script,
    mesh,
    texture,
    scene,
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
    for (auto& entry : std::filesystem::directory_iterator(state->gui_state->explorer_path)) {
        (entry.is_directory() ? directories : files).push_back(entry);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 7.0f, 7.0f });

    if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        int window_width = (int) ImGui::GetWindowContentRegionWidth();
        int column_count = window_width / state->gui_state->explorer_icon_size;
        window_width -= (column_count - 1) * (int) ImGui::GetStyle().ItemSpacing.x;
        column_count = max(window_width / state->gui_state->explorer_icon_size, 1);

        ImGui::Text(state->gui_state->explorer_path.c_str());
        ImGui::Separator();

        auto& style = ImGui::GetStyle();
        auto& color0 = style.Colors[ImGuiCol_Button];
        auto& color1 = style.Colors[ImGuiCol_ButtonHovered];
        auto& color2 = style.Colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_Button, { color0.x, color0.y, color0.z, 0.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { color1.x, color1.y, color1.z, 0.25f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { color2.x, color2.y, color2.z, 0.5f });

        if (ImGui::BeginChild("TableHolder", {}, false, ImGuiWindowFlags_NoScrollbar)) {
            if (ImGui::BeginTable("##ExplorerTable", column_count)) {
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
            ImGui::EndChild();
        }

        ImGui::PopStyleColor(3);

        if (ImGui::BeginPopupContextWindow()) {
            ImGui::SliderInt("Icon Size", &state->gui_state->explorer_icon_size, 25, 250);
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    ImGui::PopStyleVar();
}

void handle_file_entry(editor_state* state, const std::filesystem::path& file)
{
    const std::string path = std::filesystem::absolute(file).string();
    const ImVec2 icon_size = { (float) state->gui_state->explorer_icon_size, (float) state->gui_state->explorer_icon_size };
    const file_type file_type = classify_file(file);
    kl::dx::shader_view icon = file_icon(state, file_type);
    if (ImGui::ImageButton(path.c_str(), icon.Get(), icon_size, ImVec2(0, 1), ImVec2(1, 0))) {
        ShellExecuteA(0, 0, path.c_str(), 0, 0, SW_SHOW);
    }
    drag_file(path, file_type, icon);
    ImGui::Text(file.filename().string().c_str());
}

void handle_directory_entry(editor_state* state, const std::filesystem::path& directory, const bool is_parent_dir)
{
    const std::string path = std::filesystem::absolute(directory).string();
    const ImVec2 icon_size = { (float) state->gui_state->explorer_icon_size, (float) state->gui_state->explorer_icon_size };
    kl::dx::shader_view icon = state->gui_state->textures.folder->shader_view;
    std::error_code ignored_error = {};
    if (is_parent_dir) {
        icon = state->gui_state->textures.folder_parent->shader_view;
    }
    else if (std::filesystem::is_empty(directory, ignored_error)) {
        icon = state->gui_state->textures.folder_empty->shader_view;
    }
    if (ImGui::ImageButton(path.c_str(), icon.Get(), icon_size, ImVec2(0, 1), ImVec2(1, 0))) {
        state->gui_state->explorer_path = path;
    }
    ImGui::Text(directory.filename().string().c_str());
}

file_type classify_file(const std::filesystem::path& file)
{
    const std::string extension = file.extension().string();
    if (extension == ".h" || extension == ".cpp") {
        return file_type::code;
    }
    if (extension == ".dll") {
        return file_type::script;
    }
    if (extension == ".obj") {
        return file_type::mesh;
    }
    if (extension == ".jpg" || extension == ".png") {
        return file_type::texture;
    }
    if (extension == ".titian") {
        return file_type::scene;
    }
    return file_type::other;
}

kl::dx::shader_view file_icon(editor_state* state, file_type type)
{
    switch (type) {
    case file_type::code:
        return state->gui_state->textures.code->shader_view;
    case file_type::script:
        return state->gui_state->textures.script->shader_view;
    case file_type::mesh:
        return state->gui_state->textures.mesh->shader_view;
    case file_type::texture:
        return state->gui_state->textures.texture->shader_view;
    case file_type::scene:
        return state->gui_state->textures.scene->shader_view;
    }
    return state->gui_state->textures.file->shader_view;
}

void drag_file(const std::filesystem::path& file, const file_type type, const kl::dx::shader_view& texture)
{
    const std::string path = file.string();
    switch (type) {
    case file_type::code:
        GUI::drag_drop::write_data("CodeFile", path, texture);
        break;
    case file_type::script:
        GUI::drag_drop::write_data("ScriptFile", path, texture);
        break;
    case file_type::mesh:
        GUI::drag_drop::write_data("MeshFile", path, texture);
        break;
    case file_type::texture:
        GUI::drag_drop::write_data("TextureFile", path, texture);
        break;
    case file_type::scene:
        GUI::drag_drop::write_data("SceneFile", path, texture);
        break;
    }
}
