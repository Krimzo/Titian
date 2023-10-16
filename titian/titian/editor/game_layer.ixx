export module game_layer;

export import application_layer;
export import scene;

export namespace titian {
	class GameLayer : public Layer
	{
	public:
		kl::Object<ApplicationLayer> app_layer = nullptr;

		bool game_running = false;
		kl::Object<Scene> scene = nullptr;

		GameLayer(kl::Object<ApplicationLayer>& app_layer)
		{
			this->app_layer = app_layer;
			scene = new Scene(app_layer->gpu);
		}

		~GameLayer() override
		{}

		bool update() override
		{
			if (game_running) {
				float delta_time = app_layer->timer->delta();
				scene->update_physics(delta_time);
				scene->update_scripts();
			}
			return true;
		}
	};
}
