#include "klibrary.h"


// Class
kl::HashT::HashT()
{}

kl::HashT::HashT(const std::string& hash)
{
    if (hash.size() < 64) {
        return;
    }

    for (size_t i = 0; i < 32; i++) {
        std::stringstream stream = {};
        stream << std::hex << hash[i * 2] << hash[i * 2 + 1];

        uint32_t result = 0;
        stream >> result;
        buffer[i] = (uint8_t) result;
    }
}

uint8_t& kl::HashT::operator[](const size_t index)
{
    return buffer[index];
}

const uint8_t& kl::HashT::operator[](const size_t index) const
{
    return buffer[index];
}

bool kl::HashT::operator==(const HashT& other) const
{
    for (uint64_t i = 0; i < 32; i++) {
        if (buffer[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool kl::HashT::operator!=(const HashT& other) const
{
    return !(*this == other);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const HashT& hash)
{
    stream << std::hex << std::setfill('0');
    for (auto& value : hash.buffer) {
        stream << std::setw(2) << (int32_t) value;
    }
    return stream;
}
