#include "gui/gui_render.h"


void gui_depth_display(state_machine* state)
{
    if (ImGui::Begin("Depth display", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        static int light_cascade_index = 0;

        ImGui::SliderInt("Light cascade index", &light_cascade_index, 0, kl::directional_light::CASCADE_COUNT - 1);

        if (state->scene->directional_light) {
            ImGui::Image(state->scene->directional_light->get_shader_view(light_cascade_index), ImGui::GetWindowContentRegionMax());
        }
    }
    ImGui::End();
}
