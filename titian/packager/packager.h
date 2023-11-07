#pragma once

#include "serialization/serializer.h"


namespace packager {
	inline const std::string PACKAGE_EXTENSION = ".tipk";

	inline const std::string BUILTIN_DIR = "builtin";
	inline const std::string PREVIEW_DIR = "preview";

	inline const std::string PACKAGED_BUILTIN_FILE = BUILTIN_DIR + PACKAGE_EXTENSION;
	inline const std::string PACKAGED_PREVIEW_FILE = PREVIEW_DIR + PACKAGE_EXTENSION;
}

namespace packager {
	using namespace titian;

	std::set<std::string> list_files(const std::string_view& dir);
	bool create_package(const std::string_view& input_dir, const std::string& output_file);
	bool open_package(const std::string& input_file);
}
