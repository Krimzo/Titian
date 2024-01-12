#pragma once

#include "klibrary.h"


namespace string_defaults {
	inline std::string get_default_script()
	{
		return kl::read_file_string("builtin/defaults/default_script.chai");
	}

	inline std::string get_default_shader()
	{
		return kl::read_file_string("builtin/defaults/default_shader.hlsl");
	}
}
