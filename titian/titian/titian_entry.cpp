#include "main.h"


int titian_entry(const bool use_editor)
{
	using namespace titian;

	kl::Object<LayerStack> layer_stack;
	if (use_editor) {
		layer_stack = new TitianEditor();
	}
	else {
		layer_stack = new TitianGame();
	}

	while (layer_stack->process_layers());
	return 0;
}