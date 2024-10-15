#include "titian.h"


void titian::LayerStack::push_layer(Layer* layer)
{
	layers.push_back(layer);
}

bool titian::LayerStack::process_layers()
{
	for (Layer* layer : layers)
	{
		if (!layer->update())
			return false;
	}
	return true;
}
