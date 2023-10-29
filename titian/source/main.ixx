export module main;

export import packaging_entry;
export import titian_entry;
export import sandbox_entry;

enum ExeType
{
	SANDBOX_OPEN = -1,

	GAME_OPEN = 0,
	GAME_CREATE = 1,

	PACKAGER_OPEN = 2,
	PACKAGER_CREATE = 3,
};

export int main(const int argc, const char** argv)
{
	// Default exe type
	ExeType exe_type = SANDBOX_OPEN;

	// Parse explicit type
	if (argc >= 2) {
		try {
			exe_type = static_cast<ExeType>(std::stoi(argv[1]));
		}
		catch (std::exception&) {
		}
	}
	else {
		kl::print("Missing entry argument. Defaulting to Titian Game.");
	}

	// Init type
	switch (exe_type) {
	case SANDBOX_OPEN:
		return sandbox_entry();

	case GAME_OPEN:
		return titian_entry(false);

	case GAME_CREATE:
		return titian_entry(true);

	case PACKAGER_OPEN:
		return packaging_entry(false);

	case PACKAGER_CREATE:
		return packaging_entry(true);
	};

	// Display helper
	kl::print("SANDBOX_OPEN(-1) | GAME_OPEN(0) | GAME_CREATE(1) | PACKAGER_OPEN(2) | PACKAGER_CREATE(3)");
	return 1;
}
