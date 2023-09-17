export module gui_log_view;

export import gui_render;

void ui_colored_text(const kl::float4& color, const std::string& message);

export void gui_log_view(editor_state* state)
{
    if (ImGui::Begin("Log view")) {
        const size_t last_log_index = state->logger_state->last_log_index();

        for (size_t i = 0; i < state->logger_state->size(); i++) {
            const log_info& log_info = state->logger_state->get(i);

            ui_colored_text(state->gui_state->color_special, i == last_log_index ? ">" : " ");
            ImGui::SameLine();

            ui_colored_text(state->gui_state->color_alternative, kl::format(std::setw(3), i + 1, "."));
            ImGui::SameLine();

            ui_colored_text({ 0.85f, 0.75f, 0.75f, 1.0f }, kl::format("[", log_info.date, "]:"));
            ImGui::SameLine();

            ui_colored_text({ 0.95f, 0.95f, 0.90f, 1.0f }, log_info.message);
        }

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Clear all")) {
                state->logger_state->clear();
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
