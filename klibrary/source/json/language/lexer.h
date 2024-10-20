#pragma once

#include "json/language/standard.h"


namespace kl::json {
	struct Lexer
	{
		static void from_escaping(std::string& str);
		static char to_escaping(char c);

		static std::vector<Token> parse(const std::string_view& data);

	private:
		static void parse_string(const std::string_view& data, std::vector<Token>& tokens, size_t& i);
		static void parse_number(const std::string_view& data, std::vector<Token>& tokens, size_t& i);
		static void parse_null(const std::string_view& data, std::vector<Token>& tokens, size_t& i);
		static void parse_false(const std::string_view& data, std::vector<Token>& tokens, size_t& i);
		static void parse_true(const std::string_view& data, std::vector<Token>& tokens, size_t& i);
	};
}
