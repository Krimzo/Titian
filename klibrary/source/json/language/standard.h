#pragma once

#include "apis/apis.h"


namespace kl::json {
	struct Standard
	{
        static inline const std::string null_value = "null";
        static inline const std::string false_value = "false";
        static inline const std::string true_value = "true";

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

namespace kl::json {
	enum struct TokenType : int32_t
	{
		_NULL = 0,
		_FALSE,
		_TRUE,
		_NUMBER,
		_STRING,
		_OBJECT_START,
		_OBJECT_END,
		_ARRAY_START,
		_ARRAY_END,
	};
}

namespace kl::json {
	struct Token
	{
		TokenType type;
		std::string value;
	};
}
