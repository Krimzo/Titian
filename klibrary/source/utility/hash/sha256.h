#pragma once

#include "utility/hash/hash_t.h"


namespace kl {
    HashT hash(const void* data, uint64_t data_size);
    HashT hash(const std::string& data);

    template<typename T>
    HashT hash(const T& object)
    {
        return hash(&object, sizeof(T));
    }
}
