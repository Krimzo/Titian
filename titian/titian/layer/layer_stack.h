#pragma once

#include "layer/layer.h"


namespace titian {
	class LayerStack
	{
		std::vector<kl::Object<Layer>> m_layers = {};

	public:
		LayerStack() = default;
		virtual ~LayerStack() = default;

		void push_layer(const kl::Object<Layer>& layer);
		bool process_layers();
	};
}
