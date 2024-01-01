#pragma once

#include "utility/hash/hash_t.h"


namespace kl {
    Hash hash(const void* data, uint64_t data_size);
    Hash hash(const std::string& data);

    template<typename T>
    Hash hash(const T& object)
    {
        return hash(&object, sizeof(T));
    }
}
