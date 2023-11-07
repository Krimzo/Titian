#include "main.h"


titian::TitianGame::TitianGame()
{
    // Unpackage
    packager::open_package(packager::PACKAGED_BUILTIN_FILE);

    // Init layers
    app_layer = new AppLayer("Titian Game");
    game_layer = new GameLayer(&app_layer);
    render_layer = new RenderLayer(&game_layer);
    gui_layer = new GUILayer(&render_layer);

    // Bind
    game_layer->bind_self();

    // Init render passes
    render_layer->passes.emplace_back(new ShadowPass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new SkyboxPass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new LitPass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new DisplayPass(&game_layer, &render_layer));

    // Push layers
    push_layer(app_layer);
    push_layer(game_layer);
    push_layer(render_layer);
    push_layer(gui_layer);

    // Start entry scene
    const Serializer serializer = { "entry.titian", false };
    game_layer->scene->deserialize(&serializer, nullptr);
    game_layer->start_game();
}

titian::TitianGame::~TitianGame()
{
    gui_layer->sections.clear();
    render_layer->passes.clear();
}
