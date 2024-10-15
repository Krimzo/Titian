#pragma once

#include "serial/binary_serializer.h"
#include "serial/text_serializer.h"


namespace titian {
	int packager_entry(int argc, str* argv, bool pack);

	StringSet list_files(const StringView& input);
	bool create_package(const StringView& input, const StringView& output_file);
	bool open_package(const StringView& input_file, const StringView& output_dir);
}
