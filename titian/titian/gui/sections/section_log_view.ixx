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
        int max_display_count = 100;

		GUISectionLogView(kl::Object<ApplicationLayer>& app_layer, kl::Object<GUILayer>& gui_layer)
		{
            this->app_layer = app_layer;
            this->gui_layer = gui_layer;
        }

		~GUISectionLogView() override
		{}

		void render_gui() override
		{
            if (ImGui::Begin("Log View")) {
                auto& logs = app_layer->logs;

                int starting_index = (int) logs.size() - max_display_count;
                if (starting_index < 0) {
                    starting_index = 0;
                }

                for (size_t i = starting_index; i < logs.size(); i++) {
                    const LogInfo& log_info = logs[i];

                    gui_colored_text(gui_layer->alternate_color, kl::format(std::setw(3), i + 1, "."));
                    ImGui::SameLine();

                    gui_colored_text({ 0.85f, 0.75f, 0.75f, 1.0f }, kl::format("[", log_info.date, "]:"));
                    ImGui::SameLine();

                    gui_colored_text({ 0.95f, 0.95f, 0.90f, 1.0f }, log_info.message);
                }

                if (ImGui::BeginPopupContextWindow()) {
                    if (ImGui::MenuItem("Clear all")) {
                        logs.clear();
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
		}
	};
}
