export module main;

export import packaging_entry;
export import titian_entry;
export import sandbox_entry;

enum ExeType
{
	PACKAGER_OPEN = 0,
	PACKAGER_CREATE = 1,

	GAME_OPEN = 2,
	GAME_CREATE = 3,

	SANDBOX_OPEN = 4,
};

ExeType EXE_TYPE = SANDBOX_OPEN;

export int main(const int argc, const char** argv)
{
	if (argc >= 2) {
		try {
			EXE_TYPE = static_cast<ExeType>(std::stoi(argv[1]));
		}
		catch (std::exception&) {
		}
	}
	else {
		kl::print("Missing entry argument. Defaulting to Titian Game.");
	}

	int return_code = 0;
	switch (EXE_TYPE) {
	case PACKAGER_OPEN:
		return_code = packaging_entry(false);
		break;
	case PACKAGER_CREATE:
		return_code = packaging_entry(true);
		break;

	case GAME_OPEN:
		return_code = titian_entry(false);
		break;
	case GAME_CREATE:
		return_code = titian_entry(true);
		break;

	case SANDBOX_OPEN:
		return_code = sandbox_entry();
		break;
	};
	return return_code;
}
