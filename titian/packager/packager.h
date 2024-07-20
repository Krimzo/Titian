#pragma once

#include "serialization/serializer.h"


namespace packager {
	using namespace titian;

	inline const String PACKAGE_EXTENSION = ".tipk";
	inline const String BUILTIN_DIR = "builtin";
	inline const String PREVIEW_DIR = "preview";
	inline const String PACKAGED_BUILTIN_FILE = BUILTIN_DIR + PACKAGE_EXTENSION;
	inline const String PACKAGED_PREVIEW_FILE = PREVIEW_DIR + PACKAGE_EXTENSION;
}

namespace packager {
	using namespace titian;

	Set<String> list_files(const String& input);
	bool create_package(const String& input_dir, const String& output_file);
	bool open_package(const String& input_file, const String& output_dir);
}
