#pragma once

#include "graphics/graphics.h"


namespace kl
{
struct Mesh
{
    std::vector<Vertex> vertices;
    dx::Buffer buffer;

    Mesh( kl::GPU const& gpu );

    void reload();

private:
    GPU const& m_gpu;
};
}
