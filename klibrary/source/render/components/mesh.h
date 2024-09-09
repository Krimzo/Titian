#pragma once

#include "graphics/graphics.h"


namespace kl {
    struct Mesh
    {
        std::vector<Vertex<float>> data_buffer;
        dx::Buffer graphics_buffer;

        Mesh(kl::GPU* gpu);

        void reload();

    private:
        GPU* m_gpu = nullptr;
    };
}
