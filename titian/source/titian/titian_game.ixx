export module titian_game;

export import layer_stack;
export import application_layer;
export import game_layer;
export import scripting_layer;
export import render_layer;
export import gui_layer;

export namespace titian {
	class TitianGame : public LayerStack
	{
	public:
		TitianGame()
		{
            kl::Object app_layer = new ApplicationLayer("Titian");
            kl::Object game_layer = new GameLayer();
            kl::Object scripting_layer = new ScriptingLayer();
            kl::Object render_layer = new RenderLayer(app_layer);
            kl::Object gui_layer = new GUILayer(app_layer);

            push_layer(app_layer);
            push_layer(game_layer);
            push_layer(scripting_layer);
            push_layer(render_layer);
            push_layer(gui_layer);
		}
	};
}