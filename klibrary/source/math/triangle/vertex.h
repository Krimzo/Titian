#pragma once

#include "math/vector/vector4.h"


namespace kl
{
template<typename T>
struct Vertex_T
{
    union { Vector3<T> position; struct { T x, y, z; }; };
    Vector3<T> normal;
    Vector2<T> uv;

    constexpr Vertex_T()
    {}

    constexpr Vertex_T( Vector3<T> const& position )
        : position( position )
    {}

    constexpr Vertex_T( Vector3<T> const& position, Vector3<T> const& normal, Vector2<T> const& uv )
        : position( position ), normal( normal ), uv( uv )
    {}
};
}

namespace kl
{
using Vertex = Vertex_T<float>;
using VertexD = Vertex_T<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, Vertex_T<T> const& vertex )
{
    stream << "{" << vertex.position << ", " << vertex.normal << ", " << vertex.uv << "}";
    return stream;
}
}
