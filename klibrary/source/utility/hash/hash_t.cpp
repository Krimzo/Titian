#include "klibrary.h"


kl::Hash::Hash()
{}

kl::Hash::Hash(const std::string_view& hash)
{
    if (hash.size() < 64) {
        return;
    }

    for (size_t i = 0; i < 32; i++) {
        std::stringstream stream = {};
        stream << std::hex << hash[i * 2] << hash[i * 2 + 1];

        uint32_t result = 0;
        stream >> result;
        buffer[i] = uint8_t(result);
    }
}

uint8_t& kl::Hash::operator[](const size_t index)
{
    return buffer[index];
}

const uint8_t& kl::Hash::operator[](const size_t index) const
{
    return buffer[index];
}

bool kl::Hash::operator==(const Hash& other) const
{
    for (int i = 0; i < 32; i++) {
        if (buffer[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool kl::Hash::operator!=(const Hash& other) const
{
    return !(*this == other);
}

std::ostream& kl::operator<<(std::ostream& stream, const Hash& hash)
{
    stream << std::hex << std::setfill('0');
    for (uint8_t value : hash.buffer) {
        stream << std::setw(2) << int(value);
    }
    return stream;
}
