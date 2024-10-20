#include "klibrary.h"


kl::json::Object::Object()
{}

kl::json::Object::Object(const std::string_view& data)
{
    const auto tokens = Lexer::parse(data);
    compile(tokens.begin(), tokens.end());
}

bool kl::json::Object::compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last)
{
    if (first == last)
        return false;

    int depth = 0;
    std::optional<std::string> key;
    for (auto it = first; it != last; ++it) {
        if (depth == 1) {
            if (key) {
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
                    (*this)[key.value()] = std::move(container);
                }
                key.reset();
            }
            else if (it->type == TokenType::LIT_STRING) {
                key = it->value;
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

std::string kl::json::Object::decompile(const int depth) const
{
    if (empty())
        return format(Standard::object_start, Standard::object_end);

    size_t counter = 0;
    std::stringstream stream;
    if (depth >= 0) {
        const std::string map_depth(depth * 2, ' ');
        const std::string content_depth((depth + 1) * 2, ' ');
        stream << Standard::object_start << '\n';
        for (const auto& [key, value] : *this) {
            std::string name = key;
            Lexer::from_escaping(name);
            stream << content_depth << Standard::string << name << Standard::string;
            stream << Standard::assign << ' ';
            stream << value->decompile(depth + 1);
            if (++counter != size()) {
                stream << Standard::splitter;
            }
            stream << '\n';
        }
        stream << map_depth << Standard::object_end;
    }
    else {
        stream << Standard::object_start << ' ';
        for (const auto& [key, value] : *this) {
            std::string name = key;
            Lexer::from_escaping(name);
            stream << Standard::string << name << Standard::string;
            stream << Standard::assign << ' ';
            stream << value->decompile(-1);
            if (++counter != size()) {
                stream << Standard::splitter;
            }
            stream << ' ';
        }
        stream << Standard::object_end;
    }
    return stream.str();
}
