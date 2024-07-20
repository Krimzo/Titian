#include "main.h"


titian::GUISectionTimeInfo::GUISectionTimeInfo(const LayerPackage& package, void* editor)
    : GUISection("GUISectionTimeInfo", package)
    , editor(editor)
{}

void titian::GUISectionTimeInfo::render_gui()
{
    const TimeBomb _ = this->time_it();

    TitianEditor* editor = static_cast<TitianEditor*>(this->editor);
    kl::Timer* timer = &app_layer->timer;

    static auto display_time = [](const String& name, const float time, const float multi = 1000.0f)
    {
        float time_ms = time * multi;
        im::DragFloat(name.c_str(), &time_ms, 0.0f);
    };

    if (im::Begin("Time Info")) {
        display_time("GameTime", timer->elapsed(), 1.0f);
        display_time("FrameTime", timer->delta());

        im::Separator();
        im::Text("Layers");
        for (auto& layer : editor->layers) {
            display_time(layer->benchmark_name, layer->benchmark_time);
        }

        im::Separator();
        im::Text("Render Passes");
        for (auto& pass : render_layer->passes) {
            display_time(pass->benchmark_name, pass->benchmark_time);
        }

        im::Separator();
        im::Text("GUI Sections");
        for (auto& section : gui_layer->sections) {
            display_time(section->benchmark_name, section->benchmark_time);
        }
    }
    im::End();
}
