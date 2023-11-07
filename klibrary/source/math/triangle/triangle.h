#pragma once

#include "math/triangle/vertex.h"


namespace kl {
    class Triangle
    {
    public:
        Vertex a = {};
        Vertex b = {};
        Vertex c = {};

        // Construct
        Triangle();
        Triangle(const Vertex& a, const Vertex& b, const Vertex& c);

        // Normal
        Float3 normal() const;

        // Interpolation
        Float4 constants() const;
        Float3 weights(const Float4& constants, const Float2& point) const;
        Float3 weights(const Float3& position) const;
        Vertex interpolate_self(const Float3& weights) const;

        // Static
        static bool is_in_triangle(const Float3& weights);
        static float interpolate(const Float3& weights, const Float3& values);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const Triangle& obj);
}
