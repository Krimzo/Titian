export module titian_entry;

export import titian_game;
export import titian_editor;

constexpr bool USE_EDITOR = true;

export int main(const int argc, const char** argv)
{
	using namespace titian;

	kl::Object<LayerStack> layer_stack = nullptr;
	if constexpr (USE_EDITOR) {
		layer_stack = new TitianEditor();
	}
	else {
		layer_stack = new TitianGame();
	}

	while (layer_stack->process_layers()) {}
	return 0;
}
