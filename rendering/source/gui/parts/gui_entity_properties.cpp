#include "gui/gui_render.h"


void gui_entity_properties(state_machine* state)
{
    if (ImGui::Begin("Entity properties") && state->scene->selected_entity) {
        kl::entity& entity = *state->scene->selected_entity;

        ImGui::DragFloat3("Scale", entity.render_scale);

        kl::float3 rotation = entity.get_rotation();
        if (ImGui::DragFloat3("Rotation", rotation)) {
            entity.set_rotation(rotation);
        }

        kl::float3 position = entity.get_position();
        if (ImGui::DragFloat3("Position", position)) {
            entity.set_position(position);
        }
    }
    ImGui::End();
}
