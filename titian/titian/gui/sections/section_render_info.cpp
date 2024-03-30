#include "main.h"


titian::GUISectionRenderInfo::GUISectionRenderInfo(GameLayer* game_layer)
{
    this->game_layer = game_layer;
}

void titian::GUISectionRenderInfo::render_gui()
{
    kl::Timer* timer = &game_layer->app_layer->timer;

    if (ImGui::Begin("Render Info")) {
        const float update_elapsed = m_timer.elapsed();
        const float elapsed_t = timer->elapsed();
        const float delta_t = timer->delta();

        const float update_interval = 1.0f / updates_per_second;
        if (update_elapsed - m_last_update_time >= update_interval) {
            m_last_update_time = update_elapsed;
            frame_time_ms = delta_t * 1000.0f;
            frames_per_second = (int) (1.0f / delta_t);
        }

        float game_time = game_layer->game_running ? elapsed_t : 0.0f;
        ImGui::DragFloat("Game time [s]", &game_time, 0.0f);

        ImGui::DragFloat("Frame time [ms]", &frame_time_ms, 0.0f);
        ImGui::DragInt("FPS", &frames_per_second, 0.0f);

        ImGui::SliderInt("UPS", &updates_per_second, 1, 11);
    }
    ImGui::End();
}
