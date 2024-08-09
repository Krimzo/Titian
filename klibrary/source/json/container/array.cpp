#include "klibrary.h"


kl::json::Array::Array()
{}

kl::json::Array::Array(const std::initializer_list<Ref<Container>>& items)
{
    insert(begin(), items.begin(), items.end());
}

kl::json::Array::Array(const std::string& data)
{
    from_string(data);
}

bool kl::json::Array::from_string(std::string data, Preprocessor preprocessor)
{
    preprocessor.process(data);
    if (data.empty()) {
        return false;
    }

    if (data.size() < 2) {
        return false;
    }
    if (data.front() != Standard::array_start_literal || data.back() != Standard::array_end_literal) {
        return false;
    }

    data = data.substr(1, data.size() - 2);
    if (!data.empty() && data.back() != Standard::splitter_literal) {
        data += Standard::splitter_literal;
    }

    for (const auto& part : Parser::split_array_data(data)) {
        Ref<Container> containers[3] = { new Literal(), new Object(), new Array() };
        for (auto& container : containers) {
            if (container->from_string(part)) {
                push_back(container);
                break;
            }
        }
    }
    return true;
}

std::string kl::json::Array::to_string(const int depth) const
{
    if (empty()) {
        return format(Standard::array_start_literal, Standard::array_end_literal);
    }

    std::stringstream stream;
    stream << Standard::array_start_literal;
    for (size_t i = 0; i < size(); i++) {
        stream << at(i)->to_string(-1);
        if (i < size() - 1) {
            stream << Standard::splitter_literal << ' ';
        }
    }
    stream << Standard::array_end_literal;
    return stream.str();
}
