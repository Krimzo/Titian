#include "gui/gui_render.h"


static int light_cascade_index = 0;

void gui_depth_display(editor_state* state)
{
    if (ImGui::Begin("Depth display", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        ImGui::SliderInt("Light cascade index", &light_cascade_index, 0, (kl::directional_light::CASCADE_COUNT - 1));

        if (state->scene->directional_light) {
            ImGui::Image(state->scene->directional_light->get_shader_view(light_cascade_index).Get(), ImGui::GetWindowContentRegionMax());
        }
    }
    ImGui::End();
}
