#include "titian.h"


titian::GUISectionLogView::GUISectionLogView()
    : GUISection("GUISectionLogView")
{}

void titian::GUISectionLogView::render_gui()
{
    const TimeBomb _ = bench_time_bomb();

    GUILayer& gui_layer = Layers::get<GUILayer>();
    im::PushFont(gui_layer.roboto_font_large);

    const uint64_t unseen_count = Logger::last_log_index - last_log_index;
    const String title_extension = unseen_count > 0 ? kl::format(" [", unseen_count, "]###") : "###";

    if (im::Begin(kl::format("Log View", title_extension).data(), nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
        last_log_index = Logger::last_log_index;

        uint32_t log_index = 0;
        for (const auto& log_info : Logger::logs) {
            gui_colored_text(kl::format(std::setfill('0'), std::setw(3), log_index + 1, "."), gui_layer.special_color);
            im::SameLine();

            gui_colored_text(kl::format("[", log_info.date, "]:"), { 0.85f, 0.75f, 0.75f, 1.0f });
            im::SameLine();

            gui_colored_text(log_info.message, { 0.95f, 0.95f, 0.90f, 1.0f });
            log_index += 1;
        }

        if (im::BeginPopupContextWindow()) {
            if (im::MenuItem("Clear All")) {
                Logger::logs.clear();
            }
            im::EndPopup();
        }

        if (unseen_count > 0) {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    im::End();

    im::PopFont();
}
