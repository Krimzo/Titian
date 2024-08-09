#pragma once

#include "apis/apis.h"


namespace kl::json {
    template<typename K, typename V>
    using ObjectStorageType = std::map<K, V>;

	template<typename T>
	using ArrayStorageType = std::vector<T>;
}

namespace kl::json {
	struct Standard
	{
        static constexpr const char* null_value = "null";
        static constexpr const char* false_value = "false";
        static constexpr const char* true_value = "true";

        static constexpr char string_literal = '"';
        static constexpr char string_escaping = '\\';

        static constexpr char splitter_literal = ',';
        static constexpr char assign_literal = ':';

        static constexpr char object_start_literal = '{';
        static constexpr char object_end_literal = '}';

        static constexpr char array_start_literal = '[';
        static constexpr char array_end_literal = ']';
	};
}
