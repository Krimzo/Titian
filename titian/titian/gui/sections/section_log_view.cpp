#include "main.h"


titian::GUISectionLogView::GUISectionLogView(const LayerPackage& package)
    : GUISection("GUISectionLogView", package)
{}

void titian::GUISectionLogView::render_gui()
{
    const TimeBomb _ = this->time_it();

    imgui::PushFont(gui_layer->roboto_font_large);

    const uint64_t unseen_count = Logger::last_log_index - last_log_index;
    const std::string title_extension = unseen_count > 0 ? kl::format(" [", unseen_count, "]###") : "###";

    if (imgui::Begin(kl::format("Log View", title_extension).c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
        last_log_index = Logger::last_log_index;

        uint32_t log_counter = 1;
        for (const auto& log_info : Logger::logs) {
            gui_colored_text(kl::format(std::setfill('0'), std::setw(3), log_counter, "."), gui_layer->special_color);
            imgui::SameLine();

            gui_colored_text(kl::format("[", log_info.date, "]:"), { 0.85f, 0.75f, 0.75f, 1.0f });
            imgui::SameLine();

            gui_colored_text(log_info.message, { 0.95f, 0.95f, 0.90f, 1.0f });
            log_counter += 1;
        }

        if (imgui::BeginPopupContextWindow()) {
            if (imgui::MenuItem("Clear All")) {
                Logger::logs.clear();
            }
            imgui::EndPopup();
        }
    }
    imgui::End();

    imgui::PopFont();
}
