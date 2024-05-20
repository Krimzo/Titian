#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    class Vertex
    {
    public:
        Vector3<T> world;
        Vector2<T> texture;
        Vector3<T> normal;

        // Construct
        constexpr Vertex()
        {}

        constexpr Vertex(const Vector3<T>& world)
            : world(world)
        {}

        constexpr Vertex(const Vector3<T>& world, const Vector2<T>& texture)
            : world(world), texture(texture)
        {}

        constexpr Vertex(const Vector3<T>& world, const Vector3<T>& normal)
            : world(world), normal(normal)
        {}

        constexpr Vertex(const Vector3<T>& world, const Vector2<T>& texture, const Vector3<T>& normal)
            : world(world), texture(texture), normal(normal)
        {}
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Vertex<T>& vertex)
    {
        stream << "{" << vertex.world << ", " << vertex.texture << ", " << vertex.normal << "}";
        return stream;
    }
}
