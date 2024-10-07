#pragma once

#include "standard.h"


namespace titian {
	inline constexpr str _CONF_FILE = "titian.json";
	inline constexpr str CONF_EXE_TYPE = "exe_type";
	inline constexpr str CONF_ENTRY_SCENE = "entry_scene";
	inline constexpr str CONF_SPECIAL_COLOR = "special_color";
	inline constexpr str CONF_ALTERNATE_COLOR = "alternate_color";

	inline constexpr str EXE_PLAY = "play";
	inline constexpr str EXE_EDIT = "edit";
	inline constexpr str EXE_SANDBOX = "sandbox";
	inline constexpr str EXE_UNPACK = "unpack";
	inline constexpr str EXE_PACK = "pack";
	inline constexpr str EXE_VIDEO = "video";
}

namespace titian {
	inline js::Object _conf_data{ kl::read_file(_CONF_FILE) };
}
