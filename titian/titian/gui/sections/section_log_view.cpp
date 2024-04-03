#include "main.h"


titian::GUISectionLogView::GUISectionLogView(GUILayer* gui_layer)
    : GUISection("GUISectionLogView")
    , gui_layer(gui_layer)
{}

void titian::GUISectionLogView::render_gui()
{
    const TimeBomb _ = this->time_it();

    ImGui::PushFont(gui_layer->jetbrains_font);

    const uint64_t unseen_count = Logger::last_log_index - last_log_index;
    const std::string title_extension = unseen_count > 0 ? kl::format(" [", unseen_count, "]###") : "###";

    if (ImGui::Begin(kl::format("Log View", title_extension).c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
        last_log_index = Logger::last_log_index;

        uint32_t log_counter = 1;
        for (const auto& log_info : Logger::logs) {
            gui_colored_text(kl::format(std::setfill('0'), std::setw(3), log_counter, "."), gui_layer->special_color);
            ImGui::SameLine();

            gui_colored_text(kl::format("[", log_info.date, "]:"), { 0.85f, 0.75f, 0.75f, 1.0f });
            ImGui::SameLine();

            gui_colored_text(log_info.message, { 0.95f, 0.95f, 0.90f, 1.0f });
            log_counter += 1;
        }

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Clear All")) {
                Logger::logs.clear();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    ImGui::PopFont();
}
