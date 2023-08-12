#include "editor.h"


void gui_render_info(editor_state* state)
{
    if (ImGui::Begin("Render Info")) {
        const float frame_time = state->timer.delta();

        float frame_time_ms = frame_time * 1000.0f;
        ImGui::DragFloat("Frame time [ms]", &frame_time_ms, 0.0f);

        float frames_per_second = 1.0f / frame_time;
        ImGui::DragFloat("Frames per second", &frames_per_second, 0.0f);
    }
    ImGui::End();
}
