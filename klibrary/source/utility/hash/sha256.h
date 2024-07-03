#pragma once

#include "utility/hash/hash_t.h"


namespace kl {
    Hash hash(const void* data, uint64_t byte_size);

    inline Hash hash_str(const std::string_view& data)
    {
        return hash(data.data(), data.size());
    }

    template<typename T>
    Hash hash_obj(const T& object)
    {
        return hash(&object, sizeof(T));
    }
}
