#pragma once

#include "apis/apis.h"


namespace kl {
    struct Hash
    {
        uint8_t buffer[32] = {};

        Hash();
        Hash(const std::string_view& hash);

        uint8_t& operator[](size_t index);
        const uint8_t& operator[](size_t index) const;

        bool operator==(const Hash& other) const;
        bool operator!=(const Hash& other) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Hash& hash);
}
