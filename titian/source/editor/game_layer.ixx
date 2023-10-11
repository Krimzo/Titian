export module game_layer;

export import application_layer;
export import scene;

export namespace titian {
	class GameLayer : public Layer
	{
	public:
		kl::Object<ApplicationLayer> app_layer = nullptr;

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
			return true;
		}
	};
}
