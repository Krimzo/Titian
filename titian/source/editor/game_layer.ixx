export module game_layer;

export import layer;

export namespace titian {
	class GameLayer : public Layer
	{
	public:
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
