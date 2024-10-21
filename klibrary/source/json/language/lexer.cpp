#include "klibrary.h"


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

char kl::json::Lexer::to_escaping(const char c)
{
	switch (c)
	{
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 'r': return '\r';
	case 't': return '\t';
	}
	return c;
}

std::vector<kl::json::Token> kl::json::Lexer::parse(const std::string_view& data)
{
	std::vector<Token> tokens;
	tokens.reserve(data.size() / 4);
	for (size_t i = 0; i < data.size(); i++) {
		switch (data[i])
		{
		case Standard::object_start:
			tokens.emplace_back(TokenType::OBJECT_START);
			break;

		case Standard::object_end:
			tokens.emplace_back(TokenType::OBJECT_END);
			break;
			
		case Standard::array_start:
			tokens.emplace_back(TokenType::ARRAY_START);
			break;

		case Standard::array_end:
			tokens.emplace_back(TokenType::ARRAY_END);
			break;

		case Standard::string:
			parse_string(data, tokens, i);
			break;

		case '-':
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

		case Standard::true_val.front():
			parse_true(data, tokens, i);
			break;

		case Standard::false_val.front():
			parse_false(data, tokens, i);
			break;

		case Standard::null_val.front():
			parse_null(data, tokens, i);
			break;
		}
	}
	return tokens;
}

void kl::json::Lexer::parse_string(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	auto& buffer = tokens.emplace_back(TokenType::LIT_STRING).value;
	for (i += 1; i < data.size(); i++) {
		switch (data[i])
		{
		case Standard::string:
			return;

		case Standard::escaping:
			i += 1;
			if (i >= data.size())
				return;
			buffer.push_back(to_escaping(data[i]));
			break;

		default:
			buffer.push_back(data[i]);
			break;
		}
	}
}

void kl::json::Lexer::parse_number(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	auto& buffer = tokens.emplace_back(TokenType::LIT_NUMBER).value;
	for (; i < data.size(); i++) {
		switch (data[i])
		{
		case '-':
		case '.':
		case 'e':
		case 'E':
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
			buffer.push_back(data[i]);
			break;

		default:
			i -= 1;
			return;
		}
	}
}

void kl::json::Lexer::parse_null(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	if (data.substr(i, Standard::null_val.size()) == Standard::null_val) {
		tokens.emplace_back(TokenType::VAL_NULL);
		i += (int64_t) Standard::null_val.size() - 1;
	}
}

void kl::json::Lexer::parse_false(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	if (data.substr(i, Standard::false_val.size()) == Standard::false_val) {
		tokens.emplace_back(TokenType::VAL_FALSE);
		i += (int64_t) Standard::false_val.size() - 1;
	}
}

void kl::json::Lexer::parse_true(const std::string_view& data, std::vector<Token>& tokens, size_t& i)
{
	if (data.substr(i, Standard::true_val.size()) == Standard::true_val) {
		tokens.emplace_back(TokenType::VAL_TRUE);
		i += (int64_t) Standard::true_val.size() - 1;
	}
}
