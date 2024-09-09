#include "fuze.h"


titian::FuzeEditor::FuzeEditor()
{
    Layers::bind<AppLayer>(&app_layer);
    Layers::bind<VideoLayer>(&video_layer);
    Layers::bind<GUILayer>(&gui_layer);

    app_layer.init("FUZE");
    video_layer.init();
    gui_layer.init();

    gui_layer.sections.emplace_back(new GUISectionFuzeMainMenu());
    gui_layer.sections.emplace_back(new GUISectionFuzeImageEffects());
    gui_layer.sections.emplace_back(new GUISectionFuzeAudioEffects());
    gui_layer.sections.emplace_back(new GUISectionFuzeDisplay());
    gui_layer.sections.emplace_back(new GUISectionFuzeTimeline());
    gui_layer.sections.emplace_back(new GUISectionFuzeRender());

    push_layer(&app_layer);
    push_layer(&video_layer);
    push_layer(&gui_layer);
}

titian::FuzeEditor::~FuzeEditor()
{
	gui_layer.sections.clear();
}
