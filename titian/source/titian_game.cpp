#include "titian.h"


titian::TitianGame::TitianGame(const StringView& entry_scene)
{
    Layers::bind<AppLayer>(app_layer);
    Layers::bind<GameLayer>(game_layer);
    Layers::bind<RenderLayer>(render_layer);
    Layers::bind<GUILayer>(gui_layer);

	app_layer.init("Titian Game");
	game_layer.init();
	render_layer.init();
	gui_layer.init();

    render_layer.passes.emplace_back(new ShadowPass());
    render_layer.passes.emplace_back(new SkyboxPass());
    render_layer.passes.emplace_back(new ScenePass());
    render_layer.passes.emplace_back(new PostPass());
    render_layer.passes.emplace_back(new DisplayPass());

    push_layer(&app_layer);
    push_layer(&game_layer);
    push_layer(&render_layer);
    push_layer(&gui_layer);

    const Ref<Serializer> serializer = (classify_file(entry_scene) == FileType::TEXT_SCENE)
        ? (Serializer*) new TextSerializer(entry_scene, false)
        : (Serializer*) new BinarySerializer(entry_scene, false);
    if (*serializer) {
        game_layer.scene().deserialize(*serializer);
        game_layer.start_game();
    }
}

titian::TitianGame::~TitianGame()
{
    gui_layer.sections.clear();
    render_layer.passes.clear();
}
