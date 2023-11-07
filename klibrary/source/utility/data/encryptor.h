#pragma once

#include "math/math.h"


// Class
namespace kl {
    inline static constexpr int ENCRYPTOR_KEY_COUNT = 5;
    using EncryptionKey = std::vector<byte>;
}

namespace kl {
    class Encryptor : public std::array<EncryptionKey, ENCRYPTOR_KEY_COUNT>
    {
        bool key_size_exists(size_t size) const;

    public:
        Encryptor();

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
    std::ostream& operator<<(std::ostream& stream, const Encryptor& encryptor);
}
