#pragma once

#include "utility/hash/hash_t.h"


namespace kl {
    Hash hash(const void* data, uint64_t byte_size);
    Hash hash_str(const std::string_view& data);

    template<typename T>
    Hash hash_obj(const T& object)
    {
        return hash(&object, sizeof(T));
    }
}
