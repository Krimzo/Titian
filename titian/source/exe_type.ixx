export module exe_type;

export import klib;

export {
	enum class ExeType : int
	{
		GAME_OPEN = 0,
		GAME_CREATE,
		PACKAGER_UNPACK,
		PACKAGER_PACK,
		SANDBOX,

		COUNT,
	};

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
}
