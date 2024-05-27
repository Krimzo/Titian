#include "klibrary.h"


kl::ots::Preprocessor::Preprocessor()
{}

void kl::ots::Preprocessor::process(std::string& source) const
{
	apply_macros(source);
	remove_whitespace(source);
}

void kl::ots::Preprocessor::apply_macros(std::string& source) const
{
	for (const auto& [key, value] : *this)
		replace_all(source, key, value);
}

void kl::ots::Preprocessor::remove_whitespace(std::string& source) const
{
    std::string result;
    bool in_char = false;
    bool in_string = false;
    bool in_comment = false;
    for (char value : source) {
        if (value == Standard::comment_literal && !in_char && !in_string) {
            in_comment = !in_comment;
            continue;
        }
        if (in_comment)
            continue;

        if (value == Standard::char_literal && !in_string) {
            in_char = !in_char;
        }
        if (value == Standard::string_literal && !in_char) {
            in_string = !in_string;
        }

        if (in_char || in_string || !isspace(value)) {
            result.push_back(value);
        }
    }
    source = result;
}
