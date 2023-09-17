#include "klib.h"


// Private
bool kl::Encryptor::key_size_exists(const size_t size) const
{
    for (const auto& key : *this) {
        if (key.size() == size) {
            return true;
        }
    }
    return false;
}

// Public
kl::Encryptor::Encryptor()
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

void kl::Encryptor::encrypt(void* data, const size_t byte_size) const
{
    for (auto& key : *this) {
        for (size_t i = 0; i < byte_size; i++) {
            ((byte*) data)[i] ^= key[i % key.size()];
        }
    }
}

void kl::Encryptor::decrypt(void* data, const size_t byte_size) const
{
    encrypt(data, byte_size);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Encryptor& encryptor)
{
    for (int i = 0; i < ENCRYPTOR_KEY_COUNT; i++) {
        stream << "keys[" << i << "] = { ";
        for (size_t k = 0; k < (encryptor[i].size() - 1); k++) {
            stream << (int) encryptor[i][k] << ", ";
        }
        stream << (int) encryptor[i].back() << " }" << std::endl;
    }
    return stream;
}
