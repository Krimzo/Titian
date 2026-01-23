#pragma once

#include "math/vector/vector4.h"


namespace kl
{
template<typename T>
struct AABB_T
{
    union { Vector3<T> center; struct { T x, y, z; }; };
    Vector3<T> half_size;

    constexpr AABB_T()
    {
    }

    constexpr AABB_T( Vector3<T> const& center, Vector3<T> const& half_size )
        : center( center ), half_size( half_size )
    {
    }

    constexpr Vector3<T> min_point() const
    {
        return center - half_size;
    }

    constexpr Vector3<T> max_point() const
    {
        return center + half_size;
    }

    constexpr bool contains( Vector3<T> const& point ) const
    {
        const Vector3<T> minp = min_point();
        const Vector3<T> maxp = max_point();
        return
            ( point.x >= minp.x && point.x <= maxp.x ) &&
            ( point.y >= minp.y && point.y <= maxp.y ) &&
            ( point.z >= minp.z && point.z <= maxp.z );
    }
};
}

namespace kl
{
using AABB = AABB_T<float>;
using AABBD = AABB_T<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, AABB_T<T> const& aabb )
{
    stream << "{" << aabb.center << ", " << aabb.half_size << "}";
    return stream;
}
}
