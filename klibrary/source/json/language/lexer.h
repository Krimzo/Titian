#pragma once

#include "json/language/standard.h"


namespace kl::json {
	struct Lexer
	{
		static std::vector<Token> parse(const std::string& data);

		static void from_escaping(std::string& str);

	private:
		static void parse_string(const std::string& data, std::vector<Token>& tokens, size_t& i);
		static void parse_number(const std::string& data, std::vector<Token>& tokens, size_t& i);
		static void parse_null(const std::string& data, std::vector<Token>& tokens, size_t& i);
		static void parse_false(const std::string& data, std::vector<Token>& tokens, size_t& i);
		static void parse_true(const std::string& data, std::vector<Token>& tokens, size_t& i);

		static char to_escaping(char c);
	};
}
