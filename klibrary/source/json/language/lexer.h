#pragma once

#include "json/language/standard.h"


namespace kl::json
{
struct Lexer
{
    static void from_escaping( std::string& str );
    static char to_escaping( char c );

    static std::vector<Token> parse( std::string_view const& data );

private:
    static void parse_string( std::string_view const& data, std::vector<Token>& tokens, size_t& i );
    static void parse_number( std::string_view const& data, std::vector<Token>& tokens, size_t& i );
    static void parse_null( std::string_view const& data, std::vector<Token>& tokens, size_t& i );
    static void parse_false( std::string_view const& data, std::vector<Token>& tokens, size_t& i );
    static void parse_true( std::string_view const& data, std::vector<Token>& tokens, size_t& i );
};
}
