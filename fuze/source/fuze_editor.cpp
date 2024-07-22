#include "fuze.h"


fuze::FuzeEditor::FuzeEditor()
{
    // Binds
    const LayerPackage layer_package{
        &app_layer,
        nullptr,
        nullptr,
        nullptr,
        &gui_layer,
        &video_layer,
    };
    BOUND_LAYERS::bind(layer_package);
    app_layer.load_layers(layer_package);
    video_layer.load_layers(layer_package);
    gui_layer.load_layers(layer_package);

    // Init layers
    app_layer.init("FUZE");
    video_layer.init();
    gui_layer.init();

    // Init editor sections
    gui_layer.sections.emplace_back(new GUISectionVideoMainMenu(layer_package));
    gui_layer.sections.emplace_back(new GUISectionVideoEffects(layer_package));
    gui_layer.sections.emplace_back(new GUISectionVideoDisplay(layer_package));
    gui_layer.sections.emplace_back(new GUISectionVideoTimeline(layer_package));
    gui_layer.sections.emplace_back(new GUISectionVideoRender(layer_package));

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
