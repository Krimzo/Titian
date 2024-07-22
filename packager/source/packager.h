#pragma once

#include "serialization/serializer.h"


namespace packager {
	using namespace titian;
}

namespace packager {
	int packager_entry(int argc, const char** argv, bool package);

	Set<String> list_files(const String& input);
	bool create_package(const String& input, const String& output_file);
	bool open_package(const String& input_file, const String& output_dir);
}
