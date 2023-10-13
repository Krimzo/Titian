export module titian_entry;

export import titian_game;
export import titian_editor;

export int titian_entry(const bool use_editor)
{
	using namespace titian;

	kl::Object<LayerStack> layer_stack = nullptr;
	if (use_editor) {
		layer_stack = new TitianEditor();
	}
	else {
		layer_stack = new TitianGame();
	}

	while (layer_stack->process_layers()) {}
	return 0;
}
