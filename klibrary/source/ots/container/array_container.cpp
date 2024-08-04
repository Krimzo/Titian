#include "klibrary.h"


kl::ots::ArrayContainer::ArrayContainer()
{}

kl::ots::ArrayContainer::ArrayContainer(const std::string& data)
{
    from_string(data);
}

bool kl::ots::ArrayContainer::from_string(std::string data, Preprocessor preprocessor)
{
    preprocessor.process(data);
    if (data.empty()) {
        return false;
    }

    // Size/(first, last) check
    if (data.size() < 2) {
        return false;
    }
    if (data.front() != Standard::array_start_literal || data.back() != Standard::array_end_literal) {
        return false;
    }

    // Remove (first, last)
    data = data.substr(1, data.size() - 2);
    if (!data.empty() && data.back() != Standard::splitter_literal) {
        data += Standard::splitter_literal;
    }

    // Split to parts and parse each part
    for (const auto& part : Parser::split_array_data(data)) {
        Ref<DataContainer> containers[3] = { new LiteralContainer(), new ArrayContainer(), new MapContainer() };
        for (auto& container : containers) {
            if (container->from_string(part)) {
                this->push_back(container);
                break;
            }
        }
    }
    return true;
}

std::string kl::ots::ArrayContainer::to_string() const
{
    if (this->empty()) {
        return format(Standard::array_start_literal, Standard::array_end_literal);
    }

    std::stringstream stream;
    stream << Standard::array_start_literal;
    for (auto i = this->begin(); i != this->end(); i++) {
        stream << (*i)->to_string();
        if (i != --this->end()) {
            stream << Standard::splitter_literal << ' ';
        }
    }
    stream << Standard::array_end_literal;
    return stream.str();
}
