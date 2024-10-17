#pragma once

#include "graphics/graphics.h"


namespace kl {
    struct Mesh
    {
        std::vector<Vertex> vertices;
        dx::Buffer buffer;

        Mesh(kl::GPU& gpu);

        void reload();

    private:
        GPU& m_gpu;
    };
}
