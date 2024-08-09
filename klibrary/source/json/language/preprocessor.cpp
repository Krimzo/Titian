#include "klibrary.h"


kl::json::Preprocessor::Preprocessor()
{}

void kl::json::Preprocessor::process(std::string& source) const
{
	apply_macros(source);
	remove_whitespace(source);
}

void kl::json::Preprocessor::apply_macros(std::string& source) const
{
    for (const auto& [key, value] : *this) {
        replace_all(source, key, value);
    }
}

void kl::json::Preprocessor::remove_whitespace(std::string& source) const
{
    std::string result;
    bool in_string = false;
    for (const char value : source) {
        if (value == Standard::string_literal) {
            in_string = !in_string;
        }
        if (in_string || !isspace(value)) {
            result.push_back(value);
        }
    }
    source = result;
}
