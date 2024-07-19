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
			Logger::log("Usage: ... <input> <output_file>");
		}
	}
	else {
		if (argc >= 4) {
			result = open_package(argv[2], argv[3]);
		}
		else {
			Logger::log("Usage: ... <input_file> <output_dir>");
		}
	}
	return result ? 0 : 1;
}
