#pragma once

#include "apis/apis.h"


namespace kl::json {
	struct Standard
	{
        static constexpr std::string_view null_val = "null";
        static constexpr std::string_view false_val = "false";
        static constexpr std::string_view true_val = "true";

        static constexpr char string = '"';
        static constexpr char escaping = '\\';

        static constexpr char splitter = ',';
        static constexpr char assign = ':';

        static constexpr char object_start = '{';
        static constexpr char object_end = '}';

        static constexpr char array_start = '[';
        static constexpr char array_end = ']';
	};
}

namespace kl::json {
	enum struct TokenType : int32_t
	{
		VAL_NULL = 0,
		VAL_FALSE,
		VAL_TRUE,
		LIT_NUMBER,
		LIT_STRING,
		OBJECT_START,
		OBJECT_END,
		ARRAY_START,
		ARRAY_END,
	};
}

namespace kl::json {
	struct Token
	{
		TokenType type;
		std::string value;
	};
}
