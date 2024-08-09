#include "klibrary.h"


kl::json::Object::Object()
{}

kl::json::Object::Object(const std::initializer_list<std::pair<std::string, Ref<Container>>>& items)
{
    insert(items.begin(), items.end());
}

kl::json::Object::Object(const std::string& data)
{
	from_string(data);
}

bool kl::json::Object::from_string(std::string data, Preprocessor preprocessor)
{
    preprocessor.process(data);
    if (data.empty()) {
        return false;
    }

    if (data.size() < 2) {
        return false;
    }
    if (data.front() != Standard::object_start_literal || data.back() != Standard::object_end_literal) {
        return false;
    }

    data = data.substr(1, data.size() - 2);
    if (!data.empty() && data.back() != Standard::splitter_literal) {
        data += Standard::splitter_literal;
    }

    for (const auto& part : Parser::split_object_data(data)) {
        Ref<Container> containers[3] = { new Literal(), new Object(), new Array() };
        for (auto& container : containers) {
            if (container->from_string(part.second)) {
                (*this)[part.first] = container;
                break;
            }
        }
    }
    return true;
}

std::string kl::json::Object::to_string(const int depth) const
{
    if (empty()) {
        return format(Standard::object_start_literal, Standard::object_end_literal);
    }

    std::stringstream stream;
    if (depth >= 0) {
        const std::string map_depth(depth * 2, ' ');
        const std::string content_depth((depth + 1) * 2, ' ');
        const auto last_it = --end();
        stream << Standard::object_start_literal << '\n';
        for (auto i = begin(); i != end(); i++) {
            stream << content_depth << Standard::string_literal << i->first << Standard::string_literal;
            stream << Standard::assign_literal << ' ';
            stream << i->second->to_string(depth + 1);
            if (i != last_it) {
                stream << Standard::splitter_literal;
            }
            stream << '\n';
        }
        stream << map_depth << Standard::object_end_literal;
    }
    else {
        const auto last_it = --end();
        stream << Standard::object_start_literal << ' ';
        for (auto i = begin(); i != end(); i++) {
            stream << Standard::string_literal << i->first << Standard::string_literal;
            stream << Standard::assign_literal << ' ';
            stream << i->second->to_string(-1);
            if (i != last_it) {
                stream << Standard::splitter_literal;
            }
            stream << ' ';
        }
        stream << Standard::object_end_literal;
    }
    return stream.str();
}
