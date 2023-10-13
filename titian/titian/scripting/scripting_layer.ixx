export module scripting_layer;

export import game_layer;
export import layer;

export namespace titian {
	class ScriptingLayer : public Layer
	{
	public:
		kl::Object<GameLayer> game_layer = nullptr;

		ScriptingLayer(kl::Object<GameLayer>& game_layer)
		{
			this->game_layer = game_layer;
		}

		~ScriptingLayer() override
		{}
		
		bool update() override
		{
			return true;
		}
	};
}
