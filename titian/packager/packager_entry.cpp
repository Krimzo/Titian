#include "main.h"


int packager_entry(const int argc, const char** argv, const bool package)
{
	using namespace packager;

	bool result = false;
	if (package) {
		if (argc >= 4) {
			result = create_package(argv[2], argv[3]);
		}
		else {
			Logger::log("Usage: ... <input_dir> <output_file>");
		}
	}
	else {
		if (argc >= 3) {
			result = open_package(argv[2]);
		}
		else {
			Logger::log("Usage: ... <input_file>");
		}
	}
	return static_cast<int>(!result);
}
