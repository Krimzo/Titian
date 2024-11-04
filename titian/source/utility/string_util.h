#pragma once

#include "standard.h"


namespace titian {
	String get_default_script();
	String get_default_material_shader();
	String get_default_camera_shader();

	String format_byte_size(uint64_t byte_size);
	String format_path(const fs::path& path);

	String ltrim(const StringView& source, str t = " \t\n\r\f\v");
	String rtrim(const StringView& source, str t = " \t\n\r\f\v");
	String trim(const StringView& source, str t = " \t\n\r\f\v");

	bool str_find(const StringView& haystack, const StringView& needle);
}
