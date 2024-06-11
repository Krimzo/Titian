#include "main.h"


void titian::LayerStack::push_layer(Layer* layer)
{
	layers.push_back(layer);
}

bool titian::LayerStack::process_layers()
{
	for (auto& layer : layers) {
		const bool state = layer->update();
		if (!state) return false;
	}
	return true;
}
