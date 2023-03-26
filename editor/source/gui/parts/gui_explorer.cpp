#include "gui/gui_render.h"


void handle_file_entry(editor_state* state, const std::filesystem::path& file);
void handle_directory_entry(editor_state* state, const std::filesystem::path& directory);

void gui_explorer(editor_state* state)
{
    std::list<std::filesystem::path> directories = {};
    std::list<std::filesystem::path> files = {};
    for (auto& entry : std::filesystem::directory_iterator(state->gui_state.explorer_path)) {
        (entry.is_directory() ? directories : files).push_back(entry);
    }

    if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        int window_width = (int) ImGui::GetWindowContentRegionWidth();
        int column_count = window_width / state->gui_state.explorer_icon_size;
        window_width -= (column_count - 1) * (int) ImGui::GetStyle().ItemSpacing.x;
        column_count = max(window_width / state->gui_state.explorer_icon_size, 1);

        auto& style = ImGui::GetStyle();
        auto& color0 = style.Colors[ImGuiCol_Button];
        auto& color1 = style.Colors[ImGuiCol_ButtonHovered];
        auto& color2 = style.Colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_Button, { color0.x, color0.y, color0.z, 0.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { color1.x, color1.y, color1.z, 0.25f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { color2.x, color2.y, color2.z, 0.5f });

        if (ImGui::BeginTable("##ExplorerTable", column_count)) {
            for (auto& dir : directories) {
                ImGui::TableNextColumn();
                handle_directory_entry(state, dir);
            }

            for (auto& file : files) {
                ImGui::TableNextColumn();
                handle_file_entry(state, file);
            }

            ImGui::EndTable();
        }

        ImGui::PopStyleColor(3);

        if (ImGui::BeginPopupContextWindow()) {
            ImGui::SliderInt("Icon Size", &state->gui_state.explorer_icon_size, 25, 250);

            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void handle_file_entry(editor_state* state, const std::filesystem::path& file)
{
    const ImVec2 icon_size = { (float) state->gui_state.explorer_icon_size, (float) state->gui_state.explorer_icon_size };
    ImGui::ImageButton(state->gui_state.textures["file"]->shader_view.Get(), icon_size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Text(file.filename().string().c_str());
}

void handle_directory_entry(editor_state* state, const std::filesystem::path& directory)
{
    const ImVec2 icon_size = { (float) state->gui_state.explorer_icon_size, (float) state->gui_state.explorer_icon_size };
    ImGui::ImageButton(state->gui_state.textures["folder"]->shader_view.Get(), icon_size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Text(directory.filename().string().c_str());
}
