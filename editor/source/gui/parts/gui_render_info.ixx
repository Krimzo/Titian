export module gui_render_info;

export import gui_render;

export void gui_render_info(editor_state* state)
{
    if (ImGui::Begin("Render Info")) {
        static int updates_per_second = 6;
        static float frame_time_ms = 0.0f;
        static int frames_per_second = 0;

        const float elapsed_t = state->timer.elapsed();
        const float delta_t = state->timer.delta();

        if ((elapsed_t - state->gui_state->render_info.last_update_time) >= (1.0f / updates_per_second)) {
            frame_time_ms = delta_t * 1000.0f;
            frames_per_second = (int) (1.0f / delta_t);
            state->gui_state->render_info.last_update_time = elapsed_t;
        }

        ImGui::SliderInt("UPS", &updates_per_second, 1, 11);
        ImGui::DragFloat("Frame time [ms]", &frame_time_ms, 0.0f);
        ImGui::DragInt("Frames per second", &frames_per_second, 0.0f);

        float elapsed_time = state->game_running ? state->timer.elapsed() : 0.0f;
        ImGui::DragFloat("Elapsed time", &elapsed_time, 0.0f);
    }
    ImGui::End();
}
