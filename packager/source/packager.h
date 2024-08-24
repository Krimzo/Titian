#pragma once

#include "serialization/binary_serializer.h"
#include "serialization/text_serializer.h"


namespace titian {
	int packager_entry(int argc, const char** argv, bool package);

	StringSet list_files(const StringView& input);
	bool create_package(const StringView& input, const StringView& output_file);
	bool open_package(const StringView& input_file, const StringView& output_dir);
}
