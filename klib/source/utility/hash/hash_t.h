#pragma once

#include "apis/apis.h"


// Class
namespace kl {
    struct hash_t
    {
        uint8_t buffer[32] = {};

        hash_t();
        hash_t(const std::string& hash);

        uint8_t& operator[](size_t index);
        const uint8_t& operator[](size_t index) const;

        bool operator==(const hash_t& other) const;
        bool operator!=(const hash_t& other) const;
    };
}

// Format
namespace kl {
    std::ostream& operator<<(std::ostream& stream, const hash_t& hash);
}
