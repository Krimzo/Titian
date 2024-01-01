#pragma once

#include "apis/apis.h"


// Class
namespace kl {
    class Hash
    {
    public:
        uint8_t buffer[32] = {};

        Hash();
        Hash(const std::string& hash);

        uint8_t& operator[](size_t index);
        const uint8_t& operator[](size_t index) const;

        bool operator==(const Hash& other) const;
        bool operator!=(const Hash& other) const;
    };
}

// Format
namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Hash& hash);
}
