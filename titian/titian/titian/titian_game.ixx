export module titian_game;

export import layer_stack;
export import application_layer;
export import game_layer;
export import scripting_layer;
export import render_layer;
export import gui_layer;

export import skybox_pass;

export namespace titian {
	class TitianGame : public LayerStack
	{
	public:
        kl::Object<ApplicationLayer> app_layer = new ApplicationLayer("Titian");
        kl::Object<GameLayer> game_layer = new GameLayer(app_layer);
        kl::Object<ScriptingLayer> scripting_layer = new ScriptingLayer(game_layer);
        kl::Object<RenderLayer> render_layer = new RenderLayer(game_layer);
        kl::Object<GUILayer> gui_layer = new GUILayer(render_layer);

		TitianGame()
		{
            // Init render passes
            render_layer->passes.emplace_back(new SkyboxPass(game_layer, render_layer->states));

            // Push layers
            push_layer(app_layer);
            push_layer(game_layer);
            push_layer(scripting_layer);
            push_layer(render_layer);
            push_layer(gui_layer);
		}
	};
}
