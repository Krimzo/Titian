#pragma once

#include "globals.h"
#include "titian_game.h"
#include "titian_editor.h"


namespace titian {
	enum class EntryType
	{
		GAME = 0,
		GAME_EDITOR = 1,
	};
}

namespace titian {
	int titian_entry(int argc, const char** argv, const Map<String, String>& ini_data, EntryType type);
}
