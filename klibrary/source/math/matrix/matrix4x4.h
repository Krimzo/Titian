#pragma once

#include "math/matrix/matrix3x3.h"


namespace kl
{
template<typename T>
struct Matrix4x4
{
    T data[16] = {
        T( 1 ), T( 0 ), T( 0 ), T( 0 ),
        T( 0 ), T( 1 ), T( 0 ), T( 0 ),
        T( 0 ), T( 0 ), T( 1 ), T( 0 ),
        T( 0 ), T( 0 ), T( 0 ), T( 1 ),
    };

    constexpr Matrix4x4()
    {}

    constexpr T& operator[]( int index )
    {
        return data[index];
    }

    constexpr T const& operator[]( int index ) const
    {
        return data[index];
    }

    constexpr T& operator()( int x, int y )
    {
        return data[x + y * 4];
    }

    constexpr T const& operator()( int x, int y ) const
    {
        return data[x + y * 4];
    }

    constexpr T get( int index ) const
    {
        return data[index];
    }

    constexpr void set( int index, T value )
    {
        data[index] = value;
    }

    template<typename O>
    constexpr operator Matrix4x4<O>() const
    {
        Matrix4x4<O> result;
        for ( int i = 0; i < 16; i++ )
        {
            result[i] = O( data[i] );
        }
        return result;
    }

    constexpr bool operator==( Matrix4x4<T> const& other ) const
    {
        for ( int i = 0; i < 16; i++ )
        {
            if ( data[i] != other[i] )
                return false;
        }
        return true;
    }

    constexpr bool operator!=( Matrix4x4<T> const& other ) const
    {
        return !(*this == other);
    }

    constexpr Matrix4x4<T> operator+( Matrix4x4<T> const& other ) const
    {
        Matrix4x4<T> result;
        for ( int i = 0; i < 16; i++ )
            result[i] = data[i] + other[i];
        return result;
    }

    constexpr void operator+=( Matrix4x4<T> const& other )
    {
        for ( int i = 0; i < 16; i++ )
            data[i] += other[i];
    }

    constexpr Matrix4x4<T> operator-( Matrix4x4<T> const& other ) const
    {
        Matrix4x4<T> result;
        for ( int i = 0; i < 16; i++ )
            result[i] = data[i] - other[i];
        return result;
    }

    constexpr void operator-=( Matrix4x4<T> const& other )
    {
        for ( int i = 0; i < 16; i++ )
            data[i] -= other[i];
    }

    constexpr Matrix4x4<T> operator*( T value ) const
    {
        Matrix4x4<T> result;
        for ( int i = 0; i < 16; i++ )
            result[i] = data[i] * value;
        return result;
    }

    constexpr void operator*=( T value )
    {
        for ( int i = 0; i < 16; i++ )
            data[i] *= value;
    }

    constexpr Matrix4x4<T> operator*( Matrix4x4<T> const& other ) const
    {
        Matrix4x4<T> result;
        for ( int y = 0; y < 4; y++ )
        {
            for ( int x = 0; x < 4; x++ )
            {
                result( x, y ) = T( 0 );
                for ( int i = 0; i < 4; i++ )
                    result( x, y ) += (*this)(i, y) * other( x, i );
            }
        }
        return result;
    }

    constexpr void operator*=( Matrix4x4<T> const& other )
    {
        *this = *this * other;
    }

    constexpr Vector4<T> operator*( Vector4<T> const& vec ) const
    {
        Vector4<T> result;
        for ( int y = 0; y < 4; y++ )
        {
            for ( int i = 0; i < 4; i++ )
                result[y] += (*this)(i, y) * vec[i];
        }
        return result;
    }

    constexpr T determinant() const
    {
        T A2323 = (*this)(2, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(2, 3);
        T A1323 = (*this)(1, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(1, 3);
        T A1223 = (*this)(1, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(1, 3);
        T A0323 = (*this)(0, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(0, 3);
        T A0223 = (*this)(0, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(0, 3);
        T A0123 = (*this)(0, 2) * (*this)(1, 3) - (*this)(1, 2) * (*this)(0, 3);

        T result =
            (*this)(0, 0) * ((*this)(1, 1) * A2323 - (*this)(2, 1) * A1323 + (*this)(3, 1) * A1223) -
            (*this)(1, 0) * ((*this)(0, 1) * A2323 - (*this)(2, 1) * A0323 + (*this)(3, 1) * A0223) +
            (*this)(2, 0) * ((*this)(0, 1) * A1323 - (*this)(1, 1) * A0323 + (*this)(3, 1) * A0123) -
            (*this)(3, 0) * ((*this)(0, 1) * A1223 - (*this)(1, 1) * A0223 + (*this)(2, 1) * A0123);
        return T( 1 ) / result;
    }

    static constexpr Matrix4x4<T> scaling( Vector3<T> const& scale )
    {
        Matrix4x4<T> result;
        result[0] = scale.x;
        result[5] = scale.y;
        result[10] = scale.z;
        return result;
    }

    static constexpr Matrix4x4<T> rotation( Vector3<T> const& rotation )
    {
        T x_sin = sin_d( rotation.x );
        T x_cos = cos_d( rotation.x );

        Matrix4x4<T> x_rot;
        x_rot[5] = x_cos;
        x_rot[6] = -x_sin;
        x_rot[9] = x_sin;
        x_rot[10] = x_cos;

        T y_sin = sin_d( rotation.y );
        T y_cos = cos_d( rotation.y );

        Matrix4x4<T> y_rot;
        y_rot[0] = y_cos;
        y_rot[2] = y_sin;
        y_rot[8] = -y_sin;
        y_rot[10] = y_cos;

        T z_sin = sin_d( rotation.z );
        T z_cos = cos_d( rotation.z );

        Matrix4x4<T> z_rot;
        z_rot[0] = z_cos;
        z_rot[1] = -z_sin;
        z_rot[4] = z_sin;
        z_rot[5] = z_cos;

        return z_rot * y_rot * x_rot;
    }

    static constexpr Matrix4x4<T> translation( Vector3<T> const& translation )
    {
        Matrix4x4<T> result;
        result[3] = translation.x;
        result[7] = translation.y;
        result[11] = translation.z;
        return result;
    }

    static constexpr Matrix4x4<T> look_at( Vector3<T> const& position, Vector3<T> const& target, Vector3<T> const& up )
    {
        Vector3<T> f = normalize( target - position );
        Vector3<T> s = normalize( cross( up, f ) );
        Vector3<T> u = cross( f, s );

        Matrix4x4<T> result;
        result[0] = s.x;
        result[1] = s.y;
        result[2] = s.z;
        result[3] = -dot( s, position );
        result[4] = u.x;
        result[5] = u.y;
        result[6] = u.z;
        result[7] = -dot( u, position );
        result[8] = f.x;
        result[9] = f.y;
        result[10] = f.z;
        result[11] = -dot( f, position );
        return result;
    }

    static constexpr Matrix4x4<T> perspective( T field_of_view, T aspect_ratio, T near_plane, T far_plane )
    {
        T tan_half = T( 1 ) / tan_d<T>( field_of_view * 0.5f );

        Matrix4x4<T> result;
        result[0] = tan_half / aspect_ratio;
        result[5] = tan_half;
        result[10] = (-far_plane - near_plane) / (near_plane - far_plane);
        result[11] = (T( 2 ) * near_plane * far_plane) / (near_plane - far_plane);
        result[14] = T( 1 );
        result[15] = T( 0 );
        return result;
    }

    static constexpr Matrix4x4<T> orthographic( T left, T right, T bottom, T top, T near_plane, T far_plane )
    {
        Matrix4x4<T> result;
        result[0] = T( 2 ) / (right - left);
        result[5] = T( 2 ) / (top - bottom);
        result[10] = T( 2 ) / (far_plane - near_plane);
        result[3] = -(right + left) / (right - left);
        result[7] = -(top + bottom) / (top - bottom);
        result[11] = -(far_plane + near_plane) / (far_plane - near_plane);
        return result;
    }
};
}

namespace kl
{
using Int4x4 = Matrix4x4<int32_t>;
using UInt4x4 = Matrix4x4<uint32_t>;
using Float4x4 = Matrix4x4<float>;
using Double4x4 = Matrix4x4<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, Matrix4x4<T> const& mat )
{
    stream << format_matrix<4, 4, T>( mat.data );
    return stream;
}
}
