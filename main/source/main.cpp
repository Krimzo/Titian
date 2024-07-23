#include "main.h"


static constexpr bool SHOULD_PACKAGE_DEFAULTS = false;

int main(const int argc, const char** argv)
{
	using namespace titian;

	if constexpr (SHOULD_PACKAGE_DEFAULTS) {
		create_package("../titian/package/", "titian.tipk");
		create_package("../sandbox/package/", "sandbox.tipk");
		create_package("../fuze/package/", "fuze.tipk");
	}

	if (!kl::IS_DEBUG && fs::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}
	const Map<String, String> ini_data = parse_ini_file(INI_DEFAULT_FILE);

	String exe_type = EXE_PLAY;
	if (argc >= 2) {
		exe_type = argv[1];
		Logger::log("Using argument exe type: ", exe_type);
	}
	else if (ini_data.contains(INI_EXE_TYPE)) {
		exe_type = ini_data.at(INI_EXE_TYPE);
		Logger::log("Using .ini exe type: ", exe_type);
	}
	else {
		Logger::log("Exe type defaulting to ", exe_type);
	}

	/* titian */
	if (exe_type == EXE_PLAY) {
		return titian_entry(argc, argv, ini_data, EntryType::GAME);
	}
	if (exe_type == EXE_EDIT) {
		return titian_entry(argc, argv, ini_data, EntryType::GAME_EDITOR);
	}
	if (exe_type == EXE_SANDBOX) {
		return sandbox_entry(argc, argv);
	}

	// packager
	if (exe_type == EXE_UNPACK) {
		return packager_entry(argc, argv, false);
	}
	if (exe_type == EXE_PACK) {
		return packager_entry(argc, argv, true);
	}
	
	// fuze
	if (exe_type == EXE_VIDEO) {
		return fuze_entry(argc, argv);
	}

	Logger::log("Invalid exe type: ", exe_type);
	Logger::log("Exe Types: [",
		EXE_PLAY, ", ",
		EXE_EDIT, ", ",
		EXE_SANDBOX, ", ",
		EXE_UNPACK, ", ",
		EXE_PACK, ", ",
		EXE_VIDEO, "]");
	return 1;
}

static struct Ignored {
	~Ignored()
	{
		kl::console::set_enabled(true);
	}
} _ignored{};
