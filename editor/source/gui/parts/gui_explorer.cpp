#include "gui/gui_render.h"


void handle_file_entry(editor_state* state, const std::filesystem::path& file);
void handle_directory_entry(editor_state* state, const std::filesystem::path& directory);

void gui_explorer(editor_state* state)
{
    std::vector<std::filesystem::path> directories = {};
    std::vector<std::filesystem::path> files = {};
    for (auto& entry : std::filesystem::directory_iterator(state->gui_state.explorer_path)) {
        (entry.is_directory() ? directories : files).push_back(entry);
    }

    if (ImGui::Begin("Explorer")) {
        for (auto& dir : directories) {
            handle_directory_entry(state, dir);
        }
        for (auto& file : files) {
            handle_file_entry(state, file);
        }
    }
    ImGui::End();
}

void handle_file_entry(editor_state* state, const std::filesystem::path& file)
{

}

void handle_directory_entry(editor_state* state, const std::filesystem::path& directory)
{

}
