#include "utility/data/encryptor.h"


// Private
bool kl::encryptor::key_size_exists(const size_t size) const
{
    for (auto& key : *this) {
        if (key.size() == size) {
            return true;
        }
    }
    return false;
}

// Public
kl::encryptor::encryptor()
{
    for (auto& key : *this) {
        size_t key_size = 0;
        do { key_size = (size_t) rand() % 7 + 2; }
        while (key_size_exists(key_size));

        key.resize(key_size);
        for (auto& value : key) {
            value = (byte) (rand() % 256);
        }
    }
}

void kl::encryptor::encrypt(void* data, const size_t byte_size) const
{
    for (auto& key : *this) {
        for (size_t i = 0; i < byte_size; i++) {
            ((byte*) data)[i] ^= key[i % key.size()];
        }
    }
}

void kl::encryptor::decrypt(void* data, const size_t byte_size) const
{
    encrypt(data, byte_size);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const encryptor& encryptor)
{
    for (int i = 0; i < encryptor_key_count; i++) {
        stream << "keys[" << i << "] = { ";
        for (size_t k = 0; k < (encryptor[i].size() - 1); k++) {
            stream << (int) encryptor[i][k] << ", ";
        }
        stream << (int) encryptor[i].back() << " }" << std::endl;
    }
    return stream;
}
