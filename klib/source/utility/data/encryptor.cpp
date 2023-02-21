#include "utility/data/encryptor.h"


kl::encryptor::encryptor()
{
    regenerate_keys();
}

void kl::encryptor::regenerate_keys()
{
    for (auto& key : keys) {
        size_t key_size = 0;

        do {
            key_size = rand() % 7 + 2;
        }
        while ([&]
        {
            for (auto& key : keys) {
                if (key.size() == key_size) {
                    return true;
                }
            }
            return false;
        }());

        key.resize(key_size);
        for (auto& value : key) {
            value = (uint8_t) (rand() % 256);
        }
    }
}

void kl::encryptor::encrypt(void* data, const size_t size) const
{
    for (auto& key : keys) {
        for (size_t i = 0; i < size; i++) {
            ((uint8_t*) data)[i] ^= key[i % key.size()];
        }
    }
}

void kl::encryptor::decrypt(void* data, const size_t size) const
{
    encrypt(data, size);
}

std::ostream& kl::operator<<(std::ostream& os, const encryptor& encryptor)
{
    for (int i = 0; i < 5; i++) {
        os << "keys[" << i << "] = { ";
        for (size_t k = 0; k < encryptor.keys[i].size() - 1; k++) {
            os << int(encryptor.keys[i][k]) << ", ";
        }
        os << int(encryptor.keys[i].back()) << " }" << std::endl;
    }
    return os;
}
