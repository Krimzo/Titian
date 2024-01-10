#pragma once

#include "utility/string_defaults.h"


namespace string_util {
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
}
