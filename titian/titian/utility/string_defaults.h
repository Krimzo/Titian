#pragma once

#include "klibrary.h"


namespace string_defaults {
	inline const std::string DEFAULT_SCRIPT = kl::read_file_string("builtin/defaults/default_script.chai");
	inline const std::string DEFAULT_SHADER = kl::read_file_string("builtin/defaults/default_shader.hlsl");
}
