#include "gui/gui_render.h"


void gui_scene_items(state_machine* state)
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
