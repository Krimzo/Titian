export module game_layer;

export import app_layer;
export import scene;

export namespace titian {
	class GameLayer : public Layer
	{
	public:
		static GameLayer* BOUND_SELF;

		kl::Object<AppLayer> app_layer = nullptr;

		bool game_running = false;
		kl::Object<Scene> scene = nullptr;

		GameLayer(kl::Object<AppLayer>& app_layer)
		{
			this->app_layer = app_layer;
			scene = new Scene(app_layer->gpu);
		}

		~GameLayer() override
		{}

		bool update() override
		{
			if (game_running) {
				const float delta_time = app_layer->timer->delta();
				scene->update_physics(delta_time);
				scene->update_scripts();
			}
			return true;
		}

		void bind_self()
		{
			BOUND_SELF = this;
		}

		void reset_scene()
		{
			scene = new Scene(app_layer->gpu);
		}

		void start_game()
		{
			// Reload and call scripts
			app_layer->timer->reset_elapsed();
			for (auto& [_, script] : scene->scripts) {
				script->reload();
				script->call_start();
			}

			// Change state
			game_running = true;
			Logger::log("Game started.");
		}

		void stop_game()
		{
			// Change state
			game_running = false;
			Logger::log("Game stopped.");
		}
	};
}

titian::GameLayer* titian::GameLayer::BOUND_SELF = nullptr;
