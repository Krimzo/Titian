#include "main.h"


void titian::LayerStack::push_layer(const kl::Object<Layer>& layer)
{
	m_layers.push_back(layer);
}

bool titian::LayerStack::process_layers()
{
	for (auto& layer : m_layers) {
		const bool state = layer->update();
		if (!state) return false;
	}
	return true;
}
