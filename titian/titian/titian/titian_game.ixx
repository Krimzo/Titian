export module titian_game;

export import packager;
export import layer_stack;

export import app_layer;
export import game_layer;
export import render_layer;
export import gui_layer;

export import shadow_pass;
export import skybox_pass;
export import lit_pass;
export import display_pass;

export namespace titian {
	class TitianGame : public LayerStack
	{
	public:
        kl::Object<AppLayer> app_layer = nullptr;
        kl::Object<GameLayer> game_layer = nullptr;
        kl::Object<RenderLayer> render_layer = nullptr;
        kl::Object<GUILayer> gui_layer = nullptr;

		TitianGame()
		{
            // Unpackage
            open_package(PACKAGED_BUILTIN_FILE);

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

        ~TitianGame() override
        {
            gui_layer->sections.clear();
            render_layer->passes.clear();
        }
	};
}
