#pragma once

#include "klibrary.h"


namespace titian {
	inline std::string get_default_script()
	{
		return kl::read_file_string("builtin/defaults/default_script.chai");
	}

	inline std::string get_default_material_shader()
	{
		return kl::read_file_string("builtin/defaults/default_material_shader.hlsl");
	}

	inline std::string get_default_camera_shader()
	{
		return kl::read_file_string("builtin/defaults/default_camera_shader.hlsl");
	}

	inline std::string format_byte_size(uint64_t byte_size)
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

	inline std::string format_path(const std::filesystem::path& path)
	{
		return "[" + std::filesystem::absolute(path).string() + "]";
	}

	inline std::string ltrim(const std::string_view& source, const char* t = " \t\n\r\f\v")
	{
		std::string data{ source };
		data.erase(0, data.find_first_not_of(t));
		return data;
	}

	inline std::string rtrim(const std::string_view& source, const char* t = " \t\n\r\f\v")
	{
		std::string data{ source };
		data.erase(data.find_last_not_of(t) + 1);
		return data;
	}

	inline std::string trim(const std::string_view& source, const char* t = " \t\n\r\f\v")
	{
		return ltrim(rtrim(source, t), t);
	}
}
