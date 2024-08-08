#include "klibrary.h"


kl::ots::MapContainer::MapContainer()
{}

kl::ots::MapContainer::MapContainer(const std::initializer_list<std::pair<std::string, Ref<DataContainer>>>& items)
{
    insert(items.begin(), items.end());
}

kl::ots::MapContainer::MapContainer(const std::string& data)
{
	from_string(data);
}

bool kl::ots::MapContainer::from_string(std::string data, Preprocessor preprocessor)
{
    preprocessor.process(data);
    if (data.empty()) {
        return false;
    }

    // Size/(first, last) check
    if (data.size() < 2) {
        return false;
    }
    if (data.front() != Standard::map_start_literal || data.back() != Standard::map_end_literal) {
        return false;
    }

    // Remove (first, last)
    data = data.substr(1, data.size() - 2);
    if (!data.empty() && data.back() != Standard::splitter_literal) {
        data += Standard::splitter_literal;
    }

    // Split to parts and parse each part
    for (const auto& part : Parser::split_map_data(data)) {
        Ref<DataContainer> containers[3] = { new LiteralContainer(), new ArrayContainer(), new MapContainer() };
        for (auto& container : containers) {
            if (container->from_string(part.second)) {
                (*this)[part.first] = container;
                break;
            }
        }
    }
    return true;
}

std::string kl::ots::MapContainer::to_string(const int depth) const
{
    if (empty()) {
        return format(Standard::map_start_literal, Standard::map_end_literal);
    }

    std::stringstream stream;
    if (depth >= 0) {
        const std::string map_depth(depth * 2, ' ');
        const std::string content_depth((depth + 1) * 2, ' ');
        stream << Standard::map_start_literal << '\n';
        for (const auto& [key, value] : *this) {
            stream << content_depth << key;
            stream << Standard::assign_literal << ' ';
            stream << value->to_string(depth + 1);
            stream << Standard::splitter_literal << '\n';
        }
        stream << map_depth << Standard::map_end_literal;
    }
    else {
        const auto last_it = --end();
        stream << Standard::map_start_literal << ' ';
        for (auto i = begin(); i != end(); i++) {
            stream << i->first;
            stream << Standard::assign_literal << ' ';
            stream << i->second->to_string(-1);
            if (i != last_it) {
                stream << Standard::splitter_literal << ' ';
            }
        }
        stream << ' ' << Standard::map_end_literal;
    }
    return stream.str();
}
