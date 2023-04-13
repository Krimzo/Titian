#pragma once

#include "utility/hash/hash_t.h"


namespace kl {
    hash_t hash(const void* data, uint64_t data_size);
    hash_t hash(const std::string& data);

    template<typename T>
    hash_t hash(const T& object)
    {
        return hash(&object, sizeof(T));
    }
}
