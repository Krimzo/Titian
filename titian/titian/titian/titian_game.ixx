export module titian_game;

export import layer_stack;
export import app_layer;
export import game_layer;
export import render_layer;
export import gui_layer;

export import shadow_pass;
export import skybox_pass;
export import lit_pass;

export namespace titian {
	class TitianGame : public LayerStack
	{
	public:
        kl::Object<AppLayer> app_layer = new AppLayer("Titian Game");
        kl::Object<GameLayer> game_layer = new GameLayer(app_layer);
        kl::Object<RenderLayer> render_layer = new RenderLayer(game_layer);
        kl::Object<GUILayer> gui_layer = new GUILayer(render_layer);

		TitianGame()
		{
            // Binds
            game_layer->bind_self();

            // Init render passes
            render_layer->passes.emplace_back(new ShadowPass(game_layer, render_layer));
            render_layer->passes.emplace_back(new SkyboxPass(game_layer, render_layer));
			render_layer->passes.emplace_back(new LitPass(game_layer, render_layer));

            // Push layers
            push_layer(app_layer);
            push_layer(game_layer);
            push_layer(render_layer);
            push_layer(gui_layer);
		}
	};
}
