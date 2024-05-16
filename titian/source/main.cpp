#include "main.h"


static inline void display_helper()
{
	using namespace titian;

	std::stringstream stream{};
	stream << "[";
	const int last_index = static_cast<int>(ExeType::_COUNT) - 1;
	for (int i = 0; i < last_index; i++) {
		stream << static_cast<ExeType>(i) << "(" << i << "), ";
	}
	stream << static_cast<ExeType>(last_index) << "(" << last_index << ")]";
	Logger::log(stream.str());
}

int main(const int argc, const char** argv)
{
	using namespace titian;
	using namespace packager;
	using namespace sandbox;

	// Defaults
	if constexpr (SHOULD_PACKAGE_DEFAULTS) {
		create_package(BUILTIN_DIR, PACKAGED_BUILTIN_FILE);
		create_package(PREVIEW_DIR, PACKAGED_PREVIEW_FILE);
	}

	// Console
	if (!kl::IS_DEBUG && std::filesystem::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}

	// Defaults ini
	const std::unordered_map<std::string, std::string> ini_data = parse_ini_file(INI_FILE_NAME);

	// Parse explicit type
	ExeType exe_type = ExeType::GAME_OPEN;
	if (argc >= 2) {
		try {
			const std::string value = argv[1];
			exe_type = static_cast<ExeType>(std::stoi(value));
		}
		catch (std::exception) {
			display_helper();
			Logger::log("Defaulting to ", exe_type);
		}
	}
	else if (ini_data.contains(inikey::EXE_TYPE)) {
		try {
			const std::string value = ini_data.at(inikey::EXE_TYPE);
			exe_type = static_cast<ExeType>(std::stoi(value));
		}
		catch (std::exception) {
			Logger::log("Failed to parse [", inikey::EXE_TYPE, "] from [", INI_FILE_NAME, "]");
			Logger::log("Defaulting to ", exe_type);
		}
	}
	else {
		Logger::log("Usage: ... <exe_type>");
		display_helper();
		Logger::log("Defaulting to ", exe_type);
	}

	// Init type
	switch (exe_type)
	{
	case ExeType::GAME_OPEN:
		return titian_entry(argc, argv, ini_data, false);

	case ExeType::GAME_CREATE:
		return titian_entry(argc, argv, ini_data, true);

	case ExeType::PACKAGER_UNPACK:
		return packager_entry(argc, argv, false);

	case ExeType::PACKAGER_PACK:
		return packager_entry(argc, argv, true);

	case ExeType::SANDBOX:
		return sandbox_entry(argc, argv);

	default:
		display_helper();
		return 1;
	}
}

static struct Ignored {
	~Ignored()
	{
		kl::console::set_enabled(true);
	}
} _ignored{};
