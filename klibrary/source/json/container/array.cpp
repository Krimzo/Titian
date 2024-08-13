#include "klibrary.h"


kl::json::Array::Array()
{}

kl::json::Array::Array(const std::string& data)
{
    const auto tokens = Lexer::parse(data);
    compile(tokens.begin(), tokens.end());
}

bool kl::json::Array::compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last)
{
    if (first == last) {
        return false;
    }

    int depth = 0;
    for (auto it = first; it != last; ++it) {
        if (depth == 1) {
            ContainerWrap container;
            if (it->type == TokenType::_ARRAY_START) {
                container = Wrap<Array>::make();
            }
            else if (it->type == TokenType::_OBJECT_START) {
                container = Wrap<Object>::make();
            }
            else {
                container = Wrap<Literal>::make();
            }
            if (container->compile(it, last)) {
                self.push_back(std::move(container));
            }
        }
        if (it->type == TokenType::_OBJECT_START || it->type == TokenType::_ARRAY_START) {
            depth += 1;
        }
        else if (it->type == TokenType::_OBJECT_END || it->type == TokenType::_ARRAY_END) {
            depth -= 1;
            if (depth <= 0) {
                break;
            }
        }
    }
    return true;
}

std::string kl::json::Array::decompile(const int depth) const
{
    if (empty()) {
        return format(Standard::array_start_literal, Standard::array_end_literal);
    }

    std::stringstream stream;
    const auto last_it = --end();
    stream << Standard::array_start_literal;
    for (auto it = begin(); it != end(); ++it) {
        stream << (*it)->decompile(-1);
        if (it != last_it) {
            stream << Standard::splitter_literal << ' ';
        }
    }
    stream << Standard::array_end_literal;
    return stream.str();
}
