#pragma once

#include "klibrary.h"


namespace titian {
	inline std::string get_default_script()
	{
		return kl::read_file_string("builtin/defaults/default_script.chai");
	}

	inline std::string get_default_shader()
	{
		return kl::read_file_string("builtin/defaults/default_shader.hlsl");
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
}
