#pragma once

#include "serial/binary_serializer.h"
#include "serial/text_serializer.h"


namespace titian {
	int packager_entry(int argc, str* argv, bool pack);

	StringSet list_files(const StringRef& input);
	bool create_package(const StringRef& input, const StringRef& output_file);
	bool open_package(const StringRef& input_file, const StringRef& output_dir);
}
