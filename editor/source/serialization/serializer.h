#pragma once

#include "klib.h"


class serializer
{
public:
    static inline const uint32_t SERIAL_VERSION = 0x27072023;
    static inline const std::string SERIAL_VERSION_NAME = kl::format(std::hex, "0x", SERIAL_VERSION);

    const std::string path = {};
    
    serializer(const std::string& path);

    bool write_scene(const kl::object<kl::scene>& scene);
    std::pair<uint32_t, kl::object<kl::scene>> read_scene(kl::gpu* gpu);
};
