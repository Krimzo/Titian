export module section_log_view;

export import gui_section;
export import application_layer;
export import gui_layer;
export import gui_helper;

export namespace titian {
	class GUISectionLogView : public GUISection
	{
	public:
        kl::Object<ApplicationLayer> app_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;
        uint64_t last_log_index = 0;

		GUISectionLogView(kl::Object<ApplicationLayer>& app_layer, kl::Object<GUILayer>& gui_layer)
		{
            this->app_layer = app_layer;
            this->gui_layer = gui_layer;
        }

		~GUISectionLogView() override
		{}

		void render_gui() override
		{
            ImGui::PushFont(gui_layer->jetbrains_font);

            const uint64_t unseen_count = Logger::last_log_index - last_log_index;
            const std::string title_extension = unseen_count > 0 ? kl::format(" [", unseen_count, "]###") : "###";

            if (ImGui::Begin(kl::format("Log View", title_extension).c_str())) {
                last_log_index = Logger::last_log_index;

                uint32_t log_counter = 1;
                for (const auto& log_info : Logger::logs) {
                    gui_colored_text(gui_layer->alternate_color, kl::format(std::setfill('0'), std::setw(3), log_counter, "."));
                    ImGui::SameLine();

                    gui_colored_text({ 0.85f, 0.75f, 0.75f, 1.0f }, kl::format("[", log_info.date, "]:"));
                    ImGui::SameLine();

                    gui_colored_text({ 0.95f, 0.95f, 0.90f, 1.0f }, log_info.message);
                    log_counter += 1;
                }

                if (ImGui::BeginPopupContextWindow()) {
                    if (ImGui::MenuItem("Clear all")) {
                        Logger::logs.clear();
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::End();

            ImGui::PopFont();
		}
	};
}
