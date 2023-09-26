export module logging_layer;

export import layer;

export namespace titian {
	class LoggingLayer : public Layer
	{
	public:
		LoggingLayer()
		{}

		~LoggingLayer() override
		{}

		bool update() override
		{
			return true;
		}
	};
}
