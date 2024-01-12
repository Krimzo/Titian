#include "main.h"


titian::TitianGame::TitianGame(const std::string& entry_scene)
{
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
    render_layer->passes.emplace_back(new ScenePass(&game_layer, &render_layer));
    render_layer->passes.emplace_back(new DisplayPass(&game_layer, &render_layer));

    // Push layers
    push_layer(app_layer);
    push_layer(game_layer);
    push_layer(render_layer);
    push_layer(gui_layer);

    // Start entry scene
    const Serializer serializer = { entry_scene, false };
    if (serializer) {
        game_layer->scene->deserialize(&serializer, nullptr);
        game_layer->start_game();
    }
    else {
        m_is_valid = false;
    }
}

titian::TitianGame::~TitianGame()
{
    gui_layer->sections.clear();
    render_layer->passes.clear();
}

bool titian::TitianGame::is_valid() const
{
    return m_is_valid;
}
