export module layer_stack;

export import layer;

export namespace titian {
	class LayerStack
	{
		std::vector<kl::Object<Layer>> m_layers = {};

	public:
		LayerStack()
		{}

		virtual ~LayerStack()
		{}

		virtual bool process_layers() final
		{
			for (auto& layer : m_layers) {
				const bool state = layer->update();
				if (!state) return false;
			}
			return true;
		}

		void push_layer(const kl::Object<Layer>& layer)
		{
			m_layers.push_back(layer);
		}
	};
}
