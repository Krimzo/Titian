#include "klibrary.h"


kl::ots::ArrayContainerType<std::string> kl::ots::Parser::split_array_data(const std::string_view& data)
{
    ArrayContainerType<std::string> result;
    std::string builder;
    int scope_level = 0;
    bool in_char = false;
    bool in_string = false;
    for (char value : data) {
        switch (value) {
        case Standard::char_literal:
            in_char = !in_char;
            scope_level += in_char ? 1 : -1;
            break;

        case Standard::string_literal:
            in_string = !in_string;
            scope_level += in_string ? 1 : -1;
            break;

        case Standard::array_start_literal:
        case Standard::map_start_literal:
            scope_level += 1;
            break;

        case Standard::array_end_literal:
        case Standard::map_end_literal:
            scope_level -= 1;
            break;
        }

        if (value == Standard::splitter_literal && scope_level == 0) {
            result.push_back(builder);
            builder.clear();
        }
        else {
            builder.push_back(value);
        }
    }
    return result;
}

kl::ots::MapContainerType<std::string, std::string> kl::ots::Parser::split_map_data(const std::string_view& data)
{
    MapContainerType<std::string, std::string> result;
    for (const auto& part : split_array_data(data)) {
        size_t assign_index = part.find(Standard::assign_literal);
        if (assign_index == -1)
            continue;
        
        std::string key = part.substr(0, assign_index);
		std::string value = part.substr(assign_index + 1);
        result[key] = value;
    }
    return result;
}
