#pragma once

#include "math/math.h"


// Class
namespace kl {
    inline static constexpr int encryptor_key_count = 5;
    using encrpytion_key = std::vector<byte>;
}

namespace kl {
    class encryptor : public std::array<encrpytion_key, encryptor_key_count>
    {
        bool key_size_exists(size_t size) const;

    public:
        encryptor();

        void encrypt(void* data, size_t byte_size) const;
        void decrypt(void* data, size_t byte_size) const;

        template <typename T>
        void encrypt(T& obj) const
        {
            encrypt(&obj, sizeof(T));
        }

        template <typename T>
        void decrypt(T& obj) const
        {
            decrypt(&obj, sizeof(T));
        }
    };
}

// Format
namespace kl {
    std::ostream& operator<<(std::ostream& os, const encryptor& encryptor);
}
