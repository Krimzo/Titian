#include "klibrary.h"


namespace kl {
    static constexpr int MIN_KEY_SIZE = 1;
    static constexpr int MAX_KEY_SIZE = 16;
}

kl::Encryptor::Encryptor(const int key_count)
{
    this->resize(key_count);
    for (auto& key : *this) {
        key.resize(random::gen_int(MIN_KEY_SIZE, MAX_KEY_SIZE + 1));
        for (auto& value : key) {
            value = random::gen_byte();
        }
    }
}

void kl::Encryptor::run_pass(void* data, const uint64_t byte_size) const
{
    for (auto& key : *this) {
        for (uint64_t i = 0; i < byte_size; i++) {
            reinterpret_cast<byte*>(data)[i] ^= key[i % key.size()];
        }
    }
}

bool kl::Encryptor::key_size_exists(const uint64_t size) const
{
    for (const auto& key : *this) {
        if (key.size() == size) {
            return true;
        }
    }
    return false;
}

std::ostream& kl::operator<<(std::ostream& stream, const Encryptor& encryptor)
{
    for (uint64_t i = 0; i < encryptor.size(); i++) {
        stream << "keys[" << i << "] = { ";
        for (uint64_t k = 0; k < (encryptor[i].size() - 1); k++) {
            stream << int(encryptor[i][k]) << ", ";
        }
        stream << int(encryptor[i].back()) << " }" << std::endl;
    }
    return stream;
}
