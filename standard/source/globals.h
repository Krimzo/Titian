#pragma once

#include "standard.h"


namespace titian {
	inline constexpr const char* _CONF_FILE = "titian.conf";
	inline constexpr const char* CONF_EXE_TYPE = "exe_type";
	inline constexpr const char* CONF_ENTRY_SCENE = "entry_scene";
	inline constexpr const char* CONF_SPECIAL_COLOR = "special_color";
	inline constexpr const char* CONF_ALTERNATE_COLOR = "alternate_color";

	inline constexpr const char* EXE_PLAY = "play";
	inline constexpr const char* EXE_EDIT = "edit";
	inline constexpr const char* EXE_SANDBOX = "sandbox";
	inline constexpr const char* EXE_UNPACK = "unpack";
	inline constexpr const char* EXE_PACK = "pack";
	inline constexpr const char* EXE_VIDEO = "video";
}

namespace titian {
	inline ts::MapContainer _conf_data{ kl::read_file_string(_CONF_FILE) };
}
