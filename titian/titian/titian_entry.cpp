#include "main.h"


int titian_entry(const int argc, const char** argv, const bool use_editor)
{
	using namespace titian;

	bool is_valid = true;
	kl::Object<LayerStack> layer_stack;

	if (use_editor) {
		layer_stack = new TitianEditor();
	}
	else {
		std::string entry_scene = "entry.titian";
		if (argc >= 3) {
			entry_scene = argv[2];
		}
		else {
			Logger::log("Usage: ... <entry_scene>");
			Logger::log("Defaulting to ", entry_scene);
		}

		kl::Object game = new TitianGame(entry_scene);
		is_valid = game->is_valid();
		layer_stack = game;
	}
	
	if (is_valid) {
		while (layer_stack->process_layers());
	}
	return 0;
}
