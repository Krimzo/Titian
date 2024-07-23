#include "titian.h"


int titian::titian_entry(const int argc, const char** argv, const Map<String, String>& ini_data, const EntryType type)
{
	Ref<LayerStack> layer_stack;

	if (type == EntryType::GAME) {
		String entry_scene = "entry.titian";
		if (argc >= 3) {
			entry_scene = argv[2];
			Logger::log("Using argument entry scene: ", entry_scene);
		}
		else if (ini_data.contains(INI_ENTRY_SCENE)) {
			entry_scene = ini_data.at(INI_ENTRY_SCENE);
			Logger::log("Using .ini entry scene: ", entry_scene);
		}
		else {
			Logger::log("Entry scene defaulting to ", entry_scene);
		}

		Ref game = new TitianGame(entry_scene);
		if (game->is_valid()) {
			layer_stack = game;
		}
	}
	else if (type == EntryType::GAME_EDITOR) {
		layer_stack = new TitianEditor();
	}
	else {
		Logger::log("Invalid entry type");
	}
	
	if (layer_stack) {
		while (layer_stack->process_layers());
	}
	return 0;
}
