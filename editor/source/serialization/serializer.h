#pragma once

#include "serialization/serialization.h"


class serializer
{
public:
    const std::string path = {};
    
    serializer(const std::string& path);
    ~serializer();

    bool write_scene(const kl::object<kl::scene>& scene);

private:
    kl::file m_file = {};

    void write_string(const std::string& data);
    void write_collider_geometry(const kl::object<kl::scene>& scene, const PxGeometryHolder& geometry_holder);
};
