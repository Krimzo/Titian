#pragma once

#include "apis/apis.h"


namespace kl::ots {
	template<typename T>
	using ArrayContainerType = std::vector<T>;

	template<typename K, typename V>
	using MapContainerType = std::unordered_map<K, V>;
}

namespace kl::ots {
	class Standard
	{
	public:
        static constexpr const char* null_value = "null";
        static constexpr const char* false_value = "false";
        static constexpr const char* true_value = "true";

        static constexpr char char_literal = '\'';
        static constexpr char string_literal = '"';
        static constexpr char comment_literal = '$';

        static constexpr char splitter_literal = ',';
        static constexpr char assign_literal = ':';

        static constexpr char array_start_literal = '[';
        static constexpr char array_end_literal = ']';

        static constexpr char map_start_literal = '{';
        static constexpr char map_end_literal = '}';
	};
}
