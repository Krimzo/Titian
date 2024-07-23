#include "fuze.h"


fuze::FuzeEditor::FuzeEditor()
{
    // Binds
    Layers::bind<AppLayer>(&app_layer);
    Layers::bind<VideoLayer>(&video_layer);
    Layers::bind<GUILayer>(&gui_layer);

    // Init layers
    app_layer.init("FUZE");
    video_layer.init();
    gui_layer.init();

    // Init editor sections
    gui_layer.sections.emplace_back(new GUISectionVideoMainMenu());
    gui_layer.sections.emplace_back(new GUISectionVideoEffects());
    gui_layer.sections.emplace_back(new GUISectionVideoDisplay());
    gui_layer.sections.emplace_back(new GUISectionVideoTimeline());
    gui_layer.sections.emplace_back(new GUISectionVideoRender());

    // Push layers
    push_layer(&app_layer);
    push_layer(&video_layer);
    push_layer(&gui_layer);

    // Logger
    Logger::set_ready();
    kl::VERIFICATION_LOGGER = Logger::log<std::string_view>;
    app_layer.timer.stop();
}

fuze::FuzeEditor::~FuzeEditor()
{
	gui_layer.sections.clear();
}
