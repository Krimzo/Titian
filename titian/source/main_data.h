#pragma once

#include "standard/standard.h"


namespace exe_type {
	using namespace titian;

	inline const String PLAY = "play";
	inline const String EDIT = "edit";
	inline const String UNPACK = "unpack";
	inline const String PACK = "pack";
	inline const String SANDBOX = "sandbox";
}

namespace ini {
	using namespace titian;

	inline constexpr const char* _FILE_NAME = "titian.ini";
	inline constexpr const char* EXE_TYPE = "exe_type";
	inline constexpr const char* ENTRY_SCENE = "entry_scene";
}
