#include "main.h"


static ExeType EXE_TYPE = ExeType::SANDBOX;
static constexpr bool PACKAGE_DEFAULTS = false;

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
	if constexpr (PACKAGE_DEFAULTS) {
		create_package(BUILTIN_DIR, PACKAGED_BUILTIN_FILE);
		create_package(PREVIEW_DIR, PACKAGED_PREVIEW_FILE);
	}

	// Console
	if (!kl::IS_DEBUG && std::filesystem::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}

	// Parse explicit type
	if (argc >= 2) {
		try {
			EXE_TYPE = static_cast<ExeType>(std::stoi(argv[1]));
		}
		catch (std::exception) {
			display_helper();
			Logger::log("Defaulting to ", EXE_TYPE);
		}
	}
	else {
		Logger::log("Usage: ... <exe_type>");
		display_helper();
		Logger::log("Defaulting to ", EXE_TYPE);
	}

	// Init type
	switch (EXE_TYPE)
	{
	case ExeType::GAME_OPEN:
		return titian_entry(argc, argv, false);

	case ExeType::GAME_CREATE:
		return titian_entry(argc, argv, true);

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
