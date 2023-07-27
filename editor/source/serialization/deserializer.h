#pragma once

#include "serialization/serialization.h"


class deserializer
{
public:
    const std::string path = {};

    deserializer(const std::string& path);
    ~deserializer();

    std::pair<uint32_t, kl::object<kl::scene>> read_scene(kl::object<kl::gpu>& gpu);

private:
    kl::file m_file = {};

    std::string read_string();
    PxGeometryHolder read_collider_geometry(const kl::object<kl::scene>& scene);
};
