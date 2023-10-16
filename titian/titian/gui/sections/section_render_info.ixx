export module section_render_info;

export import gui_section;
export import game_layer;

export namespace titian {
    class GUISectionRenderInfo : public GUISection
    {
    public:
        kl::Object<GameLayer> game_layer = nullptr;

        int updates_per_second = 6;
        float frame_time_ms = 0.0f;
        int frames_per_second = 0;

        GUISectionRenderInfo(kl::Object<GameLayer>& game_layer)
        {
            this->game_layer = game_layer;
        }

        ~GUISectionRenderInfo() override
        {}

        void render_gui() override
        {
            kl::Timer* timer = &game_layer->app_layer->timer;

            if (ImGui::Begin("Render Info")) {
                const float elapsed_t = timer->elapsed();
                const float delta_t = timer->delta();

                if (elapsed_t - m_last_update_time >= 1.0f / updates_per_second) {
                    frame_time_ms = delta_t * 1000.0f;
                    frames_per_second = (int) (1.0f / delta_t);
                    m_last_update_time = elapsed_t;
                }

                ImGui::SliderInt("UPS", &updates_per_second, 1, 11);
                ImGui::DragFloat("Frame time [ms]", &frame_time_ms, 0.0f);
                ImGui::DragInt("FPS", &frames_per_second, 0.0f);

                float elapsed_time = game_layer->game_running ? elapsed_t : 0.0f;
                ImGui::DragFloat("Elapsed time", &elapsed_time, 0.0f);
            }
            ImGui::End();
        }

    private:
        float m_last_update_time = 0.0f;
    };
}
