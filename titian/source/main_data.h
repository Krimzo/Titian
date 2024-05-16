#pragma once

#include "klibrary.h"


enum class ExeType : int
{
	GAME_OPEN = 0,
	GAME_CREATE = 1,
	PACKAGER_UNPACK = 2,
	PACKAGER_PACK = 3,
	SANDBOX = 4,
	_COUNT,
};

inline constexpr bool SHOULD_PACKAGE_DEFAULTS = false;
inline constexpr const char* INI_FILE_NAME = "titian.ini";

namespace inikey {
	inline constexpr const char* EXE_TYPE = "exe_type";
	inline constexpr const char* ENTRY_SCENE = "entry_scene";
}

inline std::ostream& operator<<(std::ostream& stream, const ExeType type)
{
	switch (type)
	{
	case ExeType::GAME_OPEN:       return stream << "Open Game";
	case ExeType::GAME_CREATE:     return stream << "Create Game";
	case ExeType::PACKAGER_UNPACK: return stream << "Packager Unpack";
	case ExeType::PACKAGER_PACK:   return stream << "Packager Pack";
	case ExeType::SANDBOX:         return stream << "Sandbox";
	}
	return stream;
}
