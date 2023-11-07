#include "main.h"


std::ostream& operator<<(std::ostream& stream, const ExeType type)
{
	switch (type)
	{
	case ExeType::SANDBOX:         return stream << "Sandbox";
	case ExeType::GAME_OPEN:       return stream << "Open Game";
	case ExeType::GAME_CREATE:     return stream << "Create Game";
	case ExeType::PACKAGER_UNPACK: return stream << "Packager Unpack";
	case ExeType::PACKAGER_PACK:   return stream << "Packager Pack";
	}
	return stream;
}
