export module gui_scene_items;

export import gui_render;

export void gui_scene_items(EditorState* state)
{
    if (ImGui::Begin("Scene items") && state->scene) {
        for (auto& [name, entity] : *state->scene) {
            if (ImGui::Selectable(name.c_str(), entity == state->scene->selected_entity)) {
                state->scene->selected_entity = entity;
            }
        }
    }
    ImGui::End();
}
