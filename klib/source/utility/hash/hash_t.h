#pragma once

#include "apis/apis.h"


// Class
namespace kl {
    class HashT
    {
    public:
        uint8_t buffer[32] = {};

        HashT();
        HashT(const std::string& hash);

        uint8_t& operator[](size_t index);
        const uint8_t& operator[](size_t index) const;

        bool operator==(const HashT& other) const;
        bool operator!=(const HashT& other) const;
    };
}

// Format
namespace kl {
    std::ostream& operator<<(std::ostream& stream, const HashT& hash);
}
