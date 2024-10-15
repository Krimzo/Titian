#pragma once

#include "layer/layer.h"


namespace titian {
	struct LayerStack
	{
		Vector<Layer*> layers;

		LayerStack() = default;
		virtual ~LayerStack() = default;

		void push_layer(Layer* layer);
		bool process_layers();
	};
}
