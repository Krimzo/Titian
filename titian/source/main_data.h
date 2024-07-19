#pragma once

#include "klibrary.h"


namespace exe_type {
	inline const std::string PLAY = "play";
	inline const std::string EDIT = "edit";
	inline const std::string UNPACK = "unpack";
	inline const std::string PACK = "pack";
	inline const std::string SANDBOX = "sandbox";
}

namespace ini {
	inline constexpr const char* _FILE_NAME = "titian.ini";
	inline constexpr const char* EXE_TYPE = "exe_type";
	inline constexpr const char* ENTRY_SCENE = "entry_scene";
}
