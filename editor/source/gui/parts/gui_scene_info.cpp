#include "gui/gui_render.h"


void gui_scene_info(editor_state* state)
{
    if (ImGui::Begin("Scene Info")) {
        int entity_count = (int) state->scene->entity_count();
        ImGui::DragInt("Entity count", &entity_count);

        if (state->scene->camera) {
            ImGui::DragFloat3("Camera position", state->scene->camera->position);

            kl::float3 camera_direction = state->scene->camera->get_forward();
            ImGui::DragFloat3("Camera direction", camera_direction);
            state->scene->camera->set_forward(camera_direction);
        }
    }
    ImGui::End();
}