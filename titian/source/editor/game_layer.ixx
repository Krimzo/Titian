export module game_layer;

export import layer;
export import scene;

export namespace titian {
	class GameLayer : public Layer
	{
	public:
		kl::Object<Scene> scene = new Scene();

		GameLayer()
		{}

		~GameLayer() override
		{}

		bool update() override
		{
			return true;
		}
	};
}
