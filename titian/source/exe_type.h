#pragma once

#include <iostream>


enum class ExeType : int
{
	GAME_OPEN = 0,
	GAME_CREATE,
	PACKAGER_UNPACK,
	PACKAGER_PACK,
	SANDBOX,

	_COUNT,
};

std::ostream& operator<<(std::ostream& stream, ExeType type);
