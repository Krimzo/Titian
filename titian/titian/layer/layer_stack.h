#pragma once

#include "layer/layer.h"


namespace titian {
	class LayerStack
	{
	public:
		std::vector<kl::Object<Layer>> layers = {};

		LayerStack() = default;
		virtual ~LayerStack() = default;

		void push_layer(const kl::Object<Layer>& layer);
		bool process_layers();
	};
}
