#pragma once

#include "serialization/binary_serializer.h"
#include "serialization/text_serializer.h"


namespace titian {
	int packager_entry(int argc, const char** argv, bool package);

	Set<String> list_files(const String& input);
	bool create_package(const String& input, const String& output_file);
	bool open_package(const String& input_file, const String& output_dir);
}
