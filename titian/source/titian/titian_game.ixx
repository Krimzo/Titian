export module titian_game;

export import layer_stack;
export import application_layer;
export import game_layer;
export import scripting_layer;
export import render_layer;

export namespace titian {
	class TitianGame : public LayerStack
	{
	public:
		TitianGame()
		{
            kl::Object application_layer = new ApplicationLayer("Titian");
            kl::Object game_layer = new GameLayer();
            kl::Object scripting_layer = new ScriptingLayer();
            kl::Object render_layer = new RenderLayer(application_layer);

            push_layer(application_layer);
            push_layer(game_layer);
            push_layer(scripting_layer);
            push_layer(render_layer);
		}
	};
}
