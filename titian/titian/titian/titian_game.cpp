#include "main.h"


titian::TitianGame::TitianGame(const String& entry_scene)
{
    // Binds
    const LayerPackage layer_package{
        &app_layer,
        &game_layer,
        nullptr,
        &render_layer,
        &gui_layer,
    };
    BOUND_LAYERS::bind(layer_package);
    app_layer.load_layers(layer_package);
    game_layer.load_layers(layer_package);
    render_layer.load_layers(layer_package);
    gui_layer.load_layers(layer_package);

    // Init layers
	app_layer.init("Titian Game");
	game_layer.init();
	render_layer.init();
	gui_layer.init();

    // Init render passes
    render_layer.passes.emplace_back(new ShadowPass(layer_package));
    render_layer.passes.emplace_back(new SkyboxPass(layer_package));
    render_layer.passes.emplace_back(new ScenePass(layer_package));
    render_layer.passes.emplace_back(new PostPass(layer_package));
    render_layer.passes.emplace_back(new DisplayPass(layer_package));

    // Push layers
    push_layer(&app_layer);
    push_layer(&game_layer);
    push_layer(&render_layer);
    push_layer(&gui_layer);

    // Start entry scene
    const Serializer serializer = { entry_scene, false };
    if (serializer) {
        game_layer.scene->deserialize(&serializer, nullptr);
        game_layer.start_game();
    }
    else {
        m_is_valid = false;
    }
}

titian::TitianGame::~TitianGame()
{
    gui_layer.sections.clear();
    render_layer.passes.clear();
}

bool titian::TitianGame::is_valid() const
{
    return m_is_valid;
}
