#pragma once

#include "json/language/standard.h"


namespace kl::json {
	struct Parser
	{
		static ObjectStorageType<std::string, std::string> split_object_data(const std::string& data);
		static ArrayStorageType<std::string> split_array_data(const std::string& data);
	};
}
