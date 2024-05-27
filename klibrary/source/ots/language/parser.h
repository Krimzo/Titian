#pragma once

#include "ots/language/standard.h"


namespace kl::ots {
	class Parser
	{
	public:
		static ArrayContainerType<std::string> split_array_data(const std::string_view& data);
		static MapContainerType<std::string, std::string> split_map_data(const std::string_view& data);
	};
}
