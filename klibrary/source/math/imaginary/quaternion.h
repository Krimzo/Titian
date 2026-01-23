#pragma once

#include "math/imaginary/complex.h"


namespace kl
{
template<typename T>
struct Quaternion_T
{
    T r = {};
    T i = {};
    T j = {};
    T k = {};

    constexpr Quaternion_T()
    {
    }

    constexpr Quaternion_T( T x, T y, T z )
        : i( x ), j( y ), k( z )
    {
    }

    constexpr Quaternion_T( T r, T i, T j, T k )
        : r( r ), i( i ), j( j ), k( k )
    {
    }

    explicit constexpr Quaternion_T( Vector3<T> const& axis, T angle )
        : r( (T) cos_d( angle * 0.5 ) )
    {
        reinterpret_cast<Vector3<T>&>( i ) = normalize( axis ) * (T) sin_d( angle * 0.5 );
    }

    constexpr T& operator[]( int index )
    {
        return ( &r )[index];
    }

    constexpr T const& operator[]( int index ) const
    {
        return ( &r )[index];
    }

    constexpr operator Vector3<T>() const
    {
        return { i, j, k };
    }

    constexpr operator Vector4<T>() const
    {
        return { i, j, k, r };
    }

    constexpr bool operator==( Quaternion_T<T> const& other ) const
    {
        return r == other.r && i == other.i && j == other.j && k == other.k;
    }

    constexpr bool operator!=( Quaternion_T<T> const& other ) const
    {
        return !( *this == other );
    }

    constexpr Quaternion_T<T> operator+( Quaternion_T<T> const& other ) const
    {
        return { r + other.r, i + other.i, j + other.j, k + other.k };
    }

    constexpr void operator+=( Quaternion_T<T> const& other )
    {
        r += other.r;
        i += other.i;
        j += other.j;
        k += other.k;
    }

    constexpr Quaternion_T<T> operator-( Quaternion_T<T> const& other ) const
    {
        return { r - other.r, i - other.i, j - other.j, k - other.k };
    }

    constexpr void operator-=( Quaternion_T<T> const& other )
    {
        r -= other.r;
        i -= other.i;
        j -= other.j;
        k -= other.k;
    }

    constexpr Quaternion_T<T> operator*( T value ) const
    {
        return { r * value, i * value, j * value, k * value };
    }

    constexpr void operator*=( T value )
    {
        r *= value;
        i *= value;
        j *= value;
        k *= value;
    }

    constexpr Quaternion_T<T> operator*( Quaternion_T<T> const& other ) const
    {
        return {
            r * other.r - i * other.i - j * other.j - k * other.k,
            r * other.i + i * other.r + j * other.k - k * other.j,
            r * other.j - i * other.k + j * other.r + k * other.i,
            r * other.k + i * other.j - j * other.i + k * other.r,
        };
    }

    constexpr void operator*=( Quaternion_T<T> const& other )
    {
        *this = *this * other;
    }

    constexpr Quaternion_T<T> operator-() const
    {
        return { -r, -i, -j, -k };
    }

    constexpr T length() const
    {
        return sqrt( r * r + i * i + j * j + k * k );
    }
};
}

namespace kl
{
using Quaternion = Quaternion_T<float>;
using QuaternionD = Quaternion_T<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, Quaternion_T<T> const& quat )
{
    const auto load_part = [&]( T const& part ) -> std::ostream&
        {
            if ( part < T( 0 ) )
                stream << " - " << ( -part );
            else
                stream << " + " << part;
            return stream;
        };

    stream << std::setprecision( 2 );
    stream << "(" << quat.r;
    load_part( quat.i ) << "i";
    load_part( quat.j ) << "j";
    load_part( quat.k ) << "k)";
    return stream;
}
}
