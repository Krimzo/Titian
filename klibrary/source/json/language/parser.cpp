#include "klibrary.h"


kl::json::ObjectStorageType<std::string, std::string> kl::json::Parser::split_object_data(const std::string& data)
{
    ObjectStorageType<std::string, std::string> result;
    for (const auto& part : split_array_data(data)) {
        int64_t assign_index = -1;
        bool was_escaping = false;
        bool in_string = false;
        for (int64_t i = 0; i < (int64_t) part.size() - 1; i++) {
            const char value = part[i];

            if (!was_escaping && value == Standard::string_literal) {
                in_string = !in_string;
            }
            if (!in_string && value == Standard::assign_literal) {
                assign_index = i;
                break;
            }

            if (was_escaping) {
                was_escaping = false;
            }
            else if (value == Standard::string_escaping) {
                was_escaping = true;
            }
        }
        if (assign_index == -1) {
            continue;
        }

        std::string key = part.substr(0, assign_index);
        if (key.size() < 2 || key.front() != Standard::string_literal || key.back() != Standard::string_literal) {
            continue;
        }
        key = key.substr(1, key.size() - 2);

        const std::string value = part.substr(assign_index + 1);
        result[key] = value;
    }
    return result;
}

kl::json::ArrayStorageType<std::string> kl::json::Parser::split_array_data(const std::string& data)
{
    ArrayStorageType<std::string> result;
    std::string builder;
    int scope_level = 0;
    bool was_escaping = false;
    bool in_string = false;
    for (const char value : data) {
        switch (value) {
        case Standard::string_literal:
            if (!was_escaping) {
                in_string = !in_string;
                scope_level += in_string ? 1 : -1;
            }
            break;

        case Standard::object_start_literal:
        case Standard::array_start_literal:
            scope_level += 1;
            break;

        case Standard::object_end_literal:
        case Standard::array_end_literal:
            scope_level -= 1;
            break;
        }

        if (scope_level == 0 && value == Standard::splitter_literal) {
            result.push_back(builder);
            builder.clear();
        }
        else {
            builder.push_back(value);
        }

        if (was_escaping) {
            was_escaping = false;
        }
        else if (value == Standard::string_escaping) {
            was_escaping = true;
        }
    }
    return result;
}
