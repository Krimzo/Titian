#pragma once

#include "globals.h"
#include "titian_game.h"
#include "titian_editor.h"


namespace titian {
	enum struct EntryType : int32_t
	{
		GAME = 0,
		GAME_EDITOR = 1,
	};
}

namespace titian {
	int titian_entry(int argc, const char** argv, EntryType type);
}
