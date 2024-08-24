#include "klibrary.h"


std::vector<kl::json::Token> kl::json::Lexer::parse(const std::string_view& data)
{
	std::vector<Token> tokens;
	for (size_t i = 0; i < data.size(); i++) {
		switch (data[i]) {
		case Standard::object_start_literal:
			tokens.push_back(Token{ .type = TokenType::_OBJECT_START });
			break;

		case Standard::object_end_literal:
			tokens.push_back(Token{ .type = TokenType::_OBJECT_END });
			break;
			
		case Standard::array_start_literal:
			tokens.push_back(Token{ .type = TokenType::_ARRAY_START });
			break;

		case Standard::array_end_literal:
			tokens.push_back(Token{ .type = TokenType::_ARRAY_END });
			break;

		case Standard::string_literal:
			parse_string(data, tokens, i);
			break;

		case '-':
		case '.':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			parse_number(data, tokens, i);
			break;

		case 't':
			parse_true(data, tokens, i);
			break;

		case 'f':
			parse_false(data, tokens, i);
			break;

		case 'n':
			parse_null(data, tokens, i);
			break;
		}
	}
	return tokens;
}

void kl::json::Lexer::parse_string(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	std::string buffer;
	int in_string = 0;
	bool was_escaping = false;
	size_t j = i;
	for (; j < data.size(); j++) {
		const bool is_escaping = (data[j] == Standard::string_escaping);
		if (was_escaping) {
			buffer.push_back(to_escaping(data[j]));
			was_escaping = false;
		}
		else {
			if (data[j] == Standard::string_literal) {
				in_string += 1;
				if (in_string >= 2) {
					break;
				}
			}
			else if (!is_escaping) {
				buffer.push_back(data[j]);
			}
			was_escaping = is_escaping;
		}
	}
	tokens.push_back(Token{ .type = TokenType::_STRING, .value = buffer });
	i = j;
}

void kl::json::Lexer::parse_number(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	std::string buffer;
	size_t j = i;
	for (; j < data.size(); j++) {
		if (data[j] == '-' || data[j] == '.' || isdigit(data[j])) {
			buffer.push_back(data[j]);
		}
		else {
			break;
		}
	}
	tokens.push_back(Token{ .type = TokenType::_NUMBER, .value = buffer });
	i = j - 1;
}

void kl::json::Lexer::parse_null(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	if (data.substr(i, Standard::null_value.size()) == Standard::null_value) {
		tokens.push_back(Token{ .type = TokenType::_NULL });
		i += (int64_t) Standard::null_value.size() - 1;
	}
}

void kl::json::Lexer::parse_false(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	if (data.substr(i, Standard::false_value.size()) == Standard::false_value) {
		tokens.push_back(Token{ .type = TokenType::_FALSE });
		i += (int64_t) Standard::false_value.size() - 1;
	}
}

void kl::json::Lexer::parse_true(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	if (data.substr(i, Standard::true_value.size()) == Standard::true_value) {
		tokens.push_back(Token{ .type = TokenType::_TRUE });
		i += (int64_t) Standard::true_value.size() - 1;
	}
}

char kl::json::Lexer::to_escaping(const char c)
{
	switch (c)
	{
	case 'b':  return '\b';
	case 'f':  return '\f';
	case 'n':  return '\n';
	case 'r':  return '\r';
	case 't':  return '\t';
	}
	return c;
}

void kl::json::Lexer::from_escaping(std::string& str)
{
	replace_all(str, "\\", "\\\\");
	replace_all(str, "\"", "\\\"");
	replace_all(str, "\b", "\\b");
	replace_all(str, "\f", "\\f");
	replace_all(str, "\n", "\\n");
	replace_all(str, "\r", "\\r");
	replace_all(str, "\t", "\\t");
}
