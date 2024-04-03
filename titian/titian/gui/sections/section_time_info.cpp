#include "main.h"


titian::GUISectionTimeInfo::GUISectionTimeInfo(void* editor)
    : GUISection("GUISectionTimeInfo")
    , editor(editor)
{}

void titian::GUISectionTimeInfo::render_gui()
{
    const TimeBomb _ = this->time_it();

    TitianEditor* editor = static_cast<TitianEditor*>(this->editor);
    GameLayer* game_layer = &editor->game_layer;
    RenderLayer* render_layer = &editor->render_layer;
    GUILayer* gui_layer = &editor->gui_layer;
    kl::Timer* timer = &game_layer->app_layer->timer;

    static auto display_time = [](const std::string_view& name, const float time, const float multi = 1000.0f)
    {
        float time_ms = time * multi;
        ImGui::DragFloat(name.data(), &time_ms, 0.0f);
    };

    if (ImGui::Begin("Time Info")) {
        display_time("GameTime", game_layer->game_running ? timer->elapsed() : 0.0f, 1.0f);
        display_time("FrameTime", timer->delta());

        ImGui::Separator();
        ImGui::Text("Layers");
        for (auto& layer : editor->layers) {
            display_time(layer->benchmark_name, layer->benchmark_time);
        }

        ImGui::Separator();
        ImGui::Text("Render Passes");
        for (auto& pass : render_layer->passes) {
            display_time(pass->benchmark_name, pass->benchmark_time);
        }

        ImGui::Separator();
        ImGui::Text("GUI Sections");
        for (auto& section : gui_layer->sections) {
            display_time(section->benchmark_name, section->benchmark_time);
        }
    }
    ImGui::End();
}
