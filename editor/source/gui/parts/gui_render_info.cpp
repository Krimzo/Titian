#include "gui/gui_render.h"


void gui_render_info(editor_state* state)
{
    if (ImGui::Begin("Render Info")) {
        float frame_time = state->timer.get_interval() * 1000.0f;
        ImGui::DragFloat("Frame time", &frame_time);
    }
    ImGui::End();
}
