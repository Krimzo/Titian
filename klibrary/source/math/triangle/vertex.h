#pragma once

#include "math/vector/float4.h"


namespace kl {
    class Vertex
    {
    public:
        Float3 world = {};
        Float2 texture = {};
        Float3 normal = {};

        // Construct
        Vertex();
        Vertex(const Float3& world);
        Vertex(const Float3& world, const Float2& texture);
        Vertex(const Float3& world, const Float3& normal);
        Vertex(const Float3& world, const Float2& texture, const Float3& normal);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Vertex& obj);
}
