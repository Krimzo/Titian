#pragma once

#include "math/vector/vector3.h"


namespace kl
{
template<typename T>
struct Vector4
{
    T x = {};
    T y = {};
    T z = {};
    T w = {};

    constexpr Vector4()
    {}

    explicit constexpr Vector4( T value )
        : x( value ), y( value ), z( value ), w( value )
    {}

    constexpr Vector4( T x, T y, T z, T w )
        : x( x ), y( y ), z( z ), w( w )
    {}

    constexpr Vector4( Vector2<T> const& vec, T z, T w )
        : x( vec.x ), y( vec.y ), z( z ), w( w )
    {}

    constexpr Vector4( T x, Vector2<T> const& vec, T w )
        : x( x ), y( vec.x ), z( vec.y ), w( w )
    {}

    constexpr Vector4( T x, T y, Vector2<T> const& vec )
        : x( x ), y( y ), z( vec.x ), w( vec.y )
    {}

    constexpr Vector4( Vector2<T> const& vec1, Vector2<T> const& vec2 )
        : x( vec1.x ), y( vec1.y ), z( vec2.x ), w( vec2.y )
    {}

    constexpr Vector4( Vector3<T> const& vec, T w )
        : x( vec.x ), y( vec.y ), z( vec.z ), w( w )
    {}

    constexpr Vector4( T x, Vector3<T> const& vec )
        : x( x ), y( vec.x ), z( vec.y ), w( vec.z )
    {}

    constexpr T& operator[]( int index )
    {
        return (&x)[index];
    }

    constexpr T const& operator[]( int index ) const
    {
        return (&x)[index];
    }

    template<typename O>
    constexpr operator Vector4<O>() const
    {
        return { O( x ), O( y ), O( z ), O( w ) };
    }

    constexpr operator Quaternion_T<T>() const
    {
        return { w, x, y, z };
    }

    constexpr operator RGB() const
    {
        return {
            (byte) clamp( x * to_byte_rgb<T>(), T( 0 ), T( 255 ) ),
            (byte) clamp( y * to_byte_rgb<T>(), T( 0 ), T( 255 ) ),
            (byte) clamp( z * to_byte_rgb<T>(), T( 0 ), T( 255 ) ),
            (byte) clamp( w * to_byte_rgb<T>(), T( 0 ), T( 255 ) ),
        };
    }

    constexpr Vector2<T>& xy()
    {
        return reinterpret_cast<Vector2<T>&>(x);
    }

    constexpr Vector2<T> const& xy() const
    {
        return reinterpret_cast<Vector2<T> const&>(x);
    }

    constexpr Vector2<T>& yz()
    {
        return reinterpret_cast<Vector2<T>&>(y);
    }

    constexpr Vector2<T> const& yz() const
    {
        return reinterpret_cast<Vector2<T> const&>(y);
    }

    constexpr Vector2<T>& zw()
    {
        return reinterpret_cast<Vector2<T>&>(z);
    }

    constexpr Vector2<T> const& zw() const
    {
        return reinterpret_cast<Vector2<T> const&>(z);
    }

    constexpr Vector3<T>& xyz()
    {
        return reinterpret_cast<Vector3<T>&>(x);
    }

    constexpr Vector3<T> const& xyz() const
    {
        return reinterpret_cast<Vector3<T> const&>(x);
    }

    constexpr Vector3<T>& yzw()
    {
        return reinterpret_cast<Vector3<T>&>(y);
    }

    constexpr Vector3<T> const& yzw() const
    {
        return reinterpret_cast<Vector3<T> const&>(y);
    }

    constexpr bool operator==( Vector4<T> const& other ) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    constexpr bool operator!=( Vector4<T> const& other ) const
    {
        return !(*this == other);
    }

    constexpr Vector4<T> operator+( Vector4<T> const& other ) const
    {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    constexpr void operator+=( Vector4<T> const& other )
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    constexpr Vector4<T> operator-( Vector4<T> const& other ) const
    {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    constexpr void operator-=( Vector4<T> const& other )
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    constexpr Vector4<T> operator*( T value ) const
    {
        return { x * value, y * value, z * value, w * value };
    }

    constexpr void operator*=( T value )
    {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
    }

    constexpr Vector4<T> operator*( Vector4<T> const& other ) const
    {
        return { x * other.x, y * other.y, z * other.z, w * other.w };
    }

    constexpr void operator*=( Vector4<T> const& other )
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
    }

    constexpr Vector4<T> operator/( T value ) const
    {
        return { x / value, y / value, z / value, w / value };
    }

    constexpr void operator/=( T value )
    {
        x /= value;
        y /= value;
        z /= value;
        w /= value;
    }

    constexpr Vector4<T> operator/( Vector4<T> const& other ) const
    {
        return { x / other.x, y / other.y, z / other.z, w / other.w };
    }

    constexpr void operator/=( Vector4<T> const& other )
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
    }

    constexpr Vector4<T> operator-() const
    {
        return { -x, -y, -z, -w };
    }

    constexpr T length() const
    {
        return sqrt( x * x + y * y + z * z + w * w );
    }
};
}

namespace kl
{
using Int4 = Vector4<int32_t>;
using UInt4 = Vector4<uint32_t>;
using Float4 = Vector4<float>;
using Double4 = Vector4<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, Vector4<T> const& vec )
{
    stream << std::setprecision( 2 );
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return stream;
}
}
