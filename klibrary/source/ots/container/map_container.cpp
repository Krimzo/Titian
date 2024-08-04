#include "klibrary.h"


kl::ots::MapContainer::MapContainer()
{}

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

std::string kl::ots::MapContainer::to_string() const
{
    if (this->empty()) {
        return format(Standard::map_start_literal, Standard::map_end_literal);
    }

    std::stringstream stream;
    stream << Standard::map_start_literal << ' ';
    for (auto i = this->begin(); i != this->end(); i++) {
        stream << i->first;
        stream << Standard::assign_literal << ' ';
        stream << i->second->to_string();
        if (i != --this->end()) {
            stream << Standard::splitter_literal << ' ';
        }
    }
    stream << ' ' << Standard::map_end_literal;
    return stream.str();
}
