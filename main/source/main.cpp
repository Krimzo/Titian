#include "main.h"


static constexpr bool SHOULD_PACKAGE_DEFAULTS = false;

int main(const int argc, const char** argv)
{
	using namespace standard;

	if constexpr (SHOULD_PACKAGE_DEFAULTS) {
		packager::create_package("../titian/package/", "titian.tipk");
		packager::create_package("../sandbox/package/", "sandbox.tipk");
	}

	if (!kl::IS_DEBUG && fs::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}
	const Map<String, String> ini_data = titian::parse_ini_file(globals::INI_DEFAULT_FILE);

	String exe_type = globals::EXE_PLAY;
	if (argc >= 2) {
		exe_type = argv[1];
		titian::Logger::log("Using argument exe type: ", exe_type);
	}
	else if (ini_data.contains(globals::INI_EXE_TYPE)) {
		exe_type = ini_data.at(globals::INI_EXE_TYPE);
		titian::Logger::log("Using .ini exe type: ", exe_type);
	}
	else {
		titian::Logger::log("Exe type defaulting to ", exe_type);
	}

	/* titian */
	if (exe_type == globals::EXE_PLAY) {
		return titian::titian_entry(argc, argv, ini_data, titian::EntryType::GAME);
	}
	if (exe_type == globals::EXE_EDIT) {
		return titian::titian_entry(argc, argv, ini_data, titian::EntryType::GAME_EDITOR);
	}
	if (exe_type == globals::EXE_SANDBOX) {
		return sandbox::sandbox_entry(argc, argv);
	}

	// packager
	if (exe_type == globals::EXE_UNPACK) {
		return packager::packager_entry(argc, argv, false);
	}
	if (exe_type == globals::EXE_PACK) {
		return packager::packager_entry(argc, argv, true);
	}
	
	// fuze
	if (exe_type == globals::EXE_VIDEO) {
		return fuze::fuze_entry(argc, argv);
	}

	titian::Logger::log("Invalid exe type: ", exe_type);
	titian::Logger::log("Exe Types: [",
		globals::EXE_PLAY, ", ",
		globals::EXE_EDIT, ", ",
		globals::EXE_SANDBOX, ", ",
		globals::EXE_UNPACK, ", ",
		globals::EXE_PACK, ", ",
		globals::EXE_VIDEO, "]");
	return 1;
}

static struct Ignored {
	~Ignored()
	{
		kl::console::set_enabled(true);
	}
} _ignored{};
