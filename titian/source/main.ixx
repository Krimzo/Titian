export module main;

export import exe_type;

export import packaging_entry;
export import titian_entry;
export import sandbox_entry;

export int main(const int argc, const char** argv)
{
	using namespace titian;

	// Console
	if (!kl::IS_DEBUG && std::filesystem::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}

	// Default exe type
	ExeType exe_type = ExeType::SANDBOX;

	// Parse explicit type
	if (argc >= 2) {
		try {
			exe_type = static_cast<ExeType>(std::stoi(argv[1]));
		}
		catch (std::exception&) {
		}
	}
	else {
		Logger::log("Missing entry argument. Defaulting to ", exe_type);
	}

	// Init type
	switch (exe_type)
	{
	case ExeType::SANDBOX:         return sandbox_entry();
	case ExeType::GAME_OPEN:       return titian_entry(false);
	case ExeType::GAME_CREATE:     return titian_entry(true);
	case ExeType::PACKAGER_UNPACK: return packaging_entry(false);
	case ExeType::PACKAGER_PACK:   return packaging_entry(true);
	}

	// Display helper
	std::stringstream stream = {};
	const int last_index = static_cast<int>(ExeType::COUNT) - 1;
	for (int i = 0; i < last_index; i++) {
		stream << static_cast<ExeType>(i) << " [" << i << "] | ";
	}
	stream << static_cast<ExeType>(last_index) << " [" << last_index << "]";
	Logger::log(stream.str());
	return 1;
}

static struct Ignored {
	~Ignored()
	{
		kl::console::set_enabled(true);
	}
} _ignored = {};
