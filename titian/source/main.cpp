#include "main.h"


static constexpr bool SHOULD_PACKAGE_DEFAULTS = false;

int main(const int argc, const char** argv)
{
	using namespace titian;
	using namespace packager;
	using namespace sandbox;

	if constexpr (SHOULD_PACKAGE_DEFAULTS) {
		create_package(BUILTIN_DIR, PACKAGED_BUILTIN_FILE);
		create_package(PREVIEW_DIR, PACKAGED_PREVIEW_FILE);
	}

	if (!kl::IS_DEBUG && std::filesystem::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}
	const std::unordered_map<std::string, std::string> ini_data = parse_ini_file(ini::_FILE_NAME);

	std::string exe_type = exe_type::PLAY;
	if (argc >= 2) {
		exe_type = argv[1];
		Logger::log("Using argument exe type: ", exe_type);
	}
	else if (ini_data.contains(ini::EXE_TYPE)) {
		exe_type = ini_data.at(ini::EXE_TYPE);
		Logger::log("Using .ini exe type: ", exe_type);
	}
	else {
		Logger::log("Exe type defaulting to ", exe_type);
	}

	if (exe_type == exe_type::PLAY) {
		return titian_entry(argc, argv, ini_data, false);
	}
	if (exe_type == exe_type::EDIT) {
		return titian_entry(argc, argv, ini_data, true);
	}
	if (exe_type == exe_type::UNPACK) {
		return packager_entry(argc, argv, false);
	}
	if (exe_type == exe_type::PACK) {
		return packager_entry(argc, argv, true);
	}
	if (exe_type == exe_type::SANDBOX) {
		return sandbox_entry(argc, argv);
	}
	
	Logger::log("Invalid exe type: ", exe_type);
	Logger::log("Exe Types: [",
		exe_type::PLAY, ", ",
		exe_type::EDIT, ", ",
		exe_type::UNPACK, ", ",
		exe_type::PACK, ", ",
		exe_type::SANDBOX, "]");
	return 1;
}

static struct Ignored {
	~Ignored()
	{
		kl::console::set_enabled(true);
	}
} _ignored{};
