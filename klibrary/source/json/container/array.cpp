#include "klibrary.h"


kl::json::Array::Array()
{}

kl::json::Array::Array(const std::string_view& data)
{
    const auto tokens = Lexer::parse(data);
    compile(tokens.begin(), tokens.end());
}

bool kl::json::Array::compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last)
{
    if (first == last)
        return false;

    int depth = 0;
    for (auto it = first; it != last; ++it) {
        if (depth == 1) {
            Ref<Container> container;
            if (it->type == TokenType::ARRAY_START) {
                container = new Array();
            }
            else if (it->type == TokenType::OBJECT_START) {
                container = new Object();
            }
            else {
                container = new Literal();
            }
            if (container->compile(it, last)) {
                push_back(std::move(container));
            }
        }
        if (it->type == TokenType::OBJECT_START || it->type == TokenType::ARRAY_START) {
            depth += 1;
        }
        else if (it->type == TokenType::OBJECT_END || it->type == TokenType::ARRAY_END) {
            if (--depth == 0)
                break;
        }
    }
    return true;
}

std::string kl::json::Array::decompile(const int depth) const
{
    if (empty())
        return format(Standard::array_start, Standard::array_end);

    std::stringstream stream;
    stream << Standard::array_start;
    for (size_t i = 0; i < size(); i++) {
        stream << (*this)[i]->decompile(-1);
        if ((i + 1) != size()) {
            stream << Standard::splitter << ' ';
        }
    }
    stream << Standard::array_end;
    return stream.str();
}
