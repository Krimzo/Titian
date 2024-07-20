#include "main.h"


int titian_entry(const int argc, const char** argv, const titian::Map<titian::String, titian::String>& ini_data, const bool use_editor)
{
	using namespace titian;

	bool is_valid = true;
	Ref<LayerStack> layer_stack;

	if (use_editor) {
		layer_stack = new TitianEditor();
	}
	else {
		String entry_scene = "entry.titian";
		if (argc >= 3) {
			entry_scene = argv[2];
			Logger::log("Using argument entry scene: ", entry_scene);
		}
		else if (ini_data.contains(ini::ENTRY_SCENE)) {
			entry_scene = ini_data.at(ini::ENTRY_SCENE);
			Logger::log("Using .ini entry scene: ", entry_scene);
		}
		else {
			Logger::log("Entry scene defaulting to ", entry_scene);
		}

		Ref game = new TitianGame(entry_scene);
		is_valid = game->is_valid();
		layer_stack = game;
	}
	
	if (is_valid) {
		while (layer_stack->process_layers());
	}
	return 0;
}
