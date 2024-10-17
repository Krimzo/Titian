#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T>
    struct Vertex_T
    {
        union { Vector3<T> position; struct { T x, y, z; }; };
        Vector3<T> normal;
        Vector2<T> uv;

        constexpr Vertex_T()
        {}

        constexpr Vertex_T(const Vector3<T>& position)
			: position(position)
        {}

        constexpr Vertex_T(const Vector3<T>& position, const Vector3<T>& normal, const Vector2<T>& uv)
            : position(position), normal(normal), uv(uv)
        {}
    };
}

namespace kl {
    using Vertex = Vertex_T<float>;
	using VertexD = Vertex_T<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Vertex_T<T>& vertex)
    {
        stream << "{" << vertex.position << ", " << vertex.normal << ", " << vertex.uv << "}";
        return stream;
    }
}
