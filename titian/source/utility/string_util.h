#pragma once

#include "standard.h"


namespace titian {
	inline String get_default_script()
	{
		return kl::read_file("package/defaults/default_script.chai");
	}

	inline String get_default_material_shader()
	{
		return kl::read_file("package/defaults/default_material_shader.hlsl");
	}

	inline String get_default_camera_shader()
	{
		return kl::read_file("package/defaults/default_camera_shader.hlsl");
	}

	inline String format_byte_size(uint64_t byte_size)
	{
		if (byte_size < 1024) {
			return std::to_string(byte_size) + " B";
		}

		byte_size /= 1024;
		if (byte_size < 1024) {
			return std::to_string(byte_size) + " KB";
		}

		byte_size /= 1024;
		if (byte_size < 1024) {
			return std::to_string(byte_size) + " MB";
		}

		byte_size /= 1024;
		return std::to_string(byte_size) + " GB";
	}

	inline String format_path(const fs::path& path)
	{
		return "[" + fs::absolute(path).string() + "]";
	}

	inline String ltrim(const StringView& source, const char* t = " \t\n\r\f\v")
	{
		String data{ source };
		data.erase(0, data.find_first_not_of(t));
		return data;
	}

	inline String rtrim(const StringView& source, const char* t = " \t\n\r\f\v")
	{
		String data{ source };
		data.erase(data.find_last_not_of(t) + 1);
		return data;
	}

	inline String trim(const StringView& source, const char* t = " \t\n\r\f\v")
	{
		return ltrim(rtrim(source, t), t);
	}
}
