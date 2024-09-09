#pragma once

#include "math/math.h"


namespace kl {
    struct Encryptor
    {
        std::vector<std::vector<byte>> keys;

        Encryptor(int key_count = 5);

        void run_pass(void* data, uint64_t byte_size) const;

        template<typename T>
        void run_pass(T* object) const
        {
            run_pass(object, sizeof(T));
        }

    private:
        bool key_size_exists(uint64_t size) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Encryptor& encryptor);
}
