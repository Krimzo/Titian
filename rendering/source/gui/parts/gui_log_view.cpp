#include "gui/gui_render.h"


void gui_log_view(state_machine* state)
{
    if (ImGui::Begin("Log view")) {
        const size_t last_log_index = state->logger.get_last_log_index();

        for (size_t i = 0; i < state->logger.size(); i++) {
            std::string log = state->logger[i];
            if (i == last_log_index) {
                log += "    <-----";
            }
            ImGui::Text(log.c_str());
        }
    }
    ImGui::End();
}
