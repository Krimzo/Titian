#include "gui/gui_render.h"


void ui_colored_text(const kl::float4& color, const std::string& message);

void gui_log_view(editor_state* state)
{
    if (ImGui::Begin("Log view")) {
        const size_t last_log_index = state->logger.last_log_index();

        for (size_t i = 0; i < state->logger.size(); i++) {
            const log_info& log_info = state->logger.get(i);

            ui_colored_text((kl::float4) kl::colors::orange, (i == last_log_index) ? ">" : " ");
            ImGui::SameLine();

            ui_colored_text({ 0.972f, 0.223f, 0.325f, 1 }, kl::format(std::setw(3), (i + 1), "."));
            ImGui::SameLine();

            ui_colored_text({ 0.552f, 0.823f, 0.278f, 1 }, kl::format("[", log_info.date, "]:"));
            ImGui::SameLine();

            ui_colored_text(kl::float4(1.0f), log_info.message);
        }

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Clear all")) {
                state->logger.clear();
            }

            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void ui_colored_text(const kl::float4& color, const std::string& message)
{
    ImGui::TextColored(*(ImVec4*) &color, message.c_str());
}
