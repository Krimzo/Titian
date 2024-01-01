#include "main.h"


static ExeType EXE_TYPE = ExeType::SANDBOX;

int main(const int argc, const char** argv)
{
	using namespace sandbox;
	using namespace titian;
	using namespace packager;

	// Console
	if (!kl::IS_DEBUG && std::filesystem::path(argv[0]).is_absolute()) {
		kl::console::set_enabled(false);
	}

	// Parse explicit type
	if (argc >= 2) {
		try {
			EXE_TYPE = static_cast<ExeType>(std::stoi(argv[1]));
		}
		catch (std::exception&) {
			Logger::log("Invalid entry argument. Defaulting to ", EXE_TYPE);
		}
	}
	else {
		Logger::log("Missing entry argument. Defaulting to ", EXE_TYPE);
	}

	// Init type
	switch (EXE_TYPE)
	{
	case ExeType::SANDBOX:         return sandbox_entry();
	case ExeType::GAME_OPEN:       return titian_entry(false);
	case ExeType::GAME_CREATE:     return titian_entry(true);
	case ExeType::PACKAGER_UNPACK: return packager_entry(false);
	case ExeType::PACKAGER_PACK:   return packager_entry(true);
	}

	// Display helper
	std::stringstream stream = {};
	const int last_index = static_cast<int>(ExeType::_COUNT) - 1;
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
