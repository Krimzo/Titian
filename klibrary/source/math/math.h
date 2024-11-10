#pragma once

#include "math/basic/basic.h"
#include "math/imaginary/complex.h"
#include "math/imaginary/quaternion.h"
#include "math/vector/vector2.h"
#include "math/vector/vector3.h"
#include "math/vector/vector4.h"
#include "math/matrix/matrix2x2.h"
#include "math/matrix/matrix3x3.h"
#include "math/matrix/matrix4x4.h"
#include "math/triangle/vertex.h"
#include "math/triangle/triangle.h"
#include "math/raytracing/aabb.h"
#include "math/raytracing/sphere.h"
#include "math/raytracing/plane.h"
#include "math/raytracing/ray.h"


namespace kl
{
template<typename T>
constexpr Complex_T<T> abs( Complex_T<T> const& num )
{
    return {
        abs( num.r ),
        abs( num.i ),
    };
}

template<typename T>
constexpr Complex_T<T> min( Complex_T<T> const& num1, Complex_T<T> const& num2 )
{
    return {
        min( num1.r, num2.r ),
        min( num1.i, num2.i ),
    };
}

template<typename T>
constexpr Complex_T<T> max( Complex_T<T> const& num1, Complex_T<T> const& num2 )
{
    return {
        max( num1.r, num2.r ),
        max( num1.i, num2.i ),
    };
}

template<typename T>
constexpr Complex_T<T> clamp( Complex_T<T> const& num, Complex_T<T> const& lower, Complex_T<T> const& upper )
{
    return {
        clamp( num.r, lower.r, upper.r ),
        clamp( num.i, lower.i, upper.i ),
    };
}

template<typename T, bool Clamp = true>
constexpr Complex_T<T> lerp( T value, Complex_T<T> const& lower, Complex_T<T> const& upper )
{
    return {
        lerp<T, Clamp>( value, lower.r, upper.r ),
        lerp<T, Clamp>( value, lower.i, upper.i ),
    };
}

template<typename T, bool Clamp = true>
constexpr Complex_T<T> unlerp( T value, Complex_T<T> const& lower, Complex_T<T> const& upper )
{
    return {
        unlerp<T, Clamp>( value, lower.r, upper.r ),
        unlerp<T, Clamp>( value, lower.i, upper.i ),
    };
}

template<typename T>
constexpr Complex_T<T> normalize( Complex_T<T> const& num )
{
    return num * (T( 1 ) / num.length());
}

template<typename T>
constexpr Complex_T<T> inverse( Complex_T<T> const& num )
{
    T sqr_sum = num.r * num.r + num.i * num.i;
    if ( sqr_sum == T( 0 ) )
        return {};
    return { num.r / sqr_sum, -num.i / sqr_sum };
}
}

namespace kl
{
template<typename T>
constexpr Quaternion_T<T> abs( Quaternion_T<T> const& num )
{
    return {
        abs( num.w ),
        abs( num.x ),
        abs( num.y ),
        abs( num.z ),
    };
}

template<typename T>
constexpr Quaternion_T<T> min( Quaternion_T<T> const& num1, Quaternion_T<T> const& num2 )
{
    return {
        min( num1.w, num2.w ),
        min( num1.x, num2.x ),
        min( num1.y, num2.y ),
        min( num1.z, num2.z ),
    };
}

template<typename T>
constexpr Quaternion_T<T> max( Quaternion_T<T> const& num1, Quaternion_T<T> const& num2 )
{
    return {
        max( num1.w, num2.w ),
        max( num1.x, num2.x ),
        max( num1.y, num2.y ),
        max( num1.z, num2.z ),
    };
}

template<typename T>
constexpr Quaternion_T<T> clamp( Quaternion_T<T> const& num, Quaternion_T<T> const& lower, Quaternion_T<T> const& upper )
{
    return {
        clamp( num.w, lower.w, upper.w ),
        clamp( num.x, lower.x, upper.x ),
        clamp( num.y, lower.y, upper.y ),
        clamp( num.z, lower.z, upper.z ),
    };
}

template<typename T, bool Clamp = true>
constexpr Quaternion_T<T> lerp( T value, Quaternion_T<T> const& lower, Quaternion_T<T> const& upper )
{
    return {
        lerp<T, Clamp>( value, lower.w, upper.w ),
        lerp<T, Clamp>( value, lower.x, upper.x ),
        lerp<T, Clamp>( value, lower.y, upper.y ),
        lerp<T, Clamp>( value, lower.z, upper.z ),
    };
}

template<typename T, bool Clamp = true>
constexpr Quaternion_T<T> unlerp( T value, Quaternion_T<T> const& lower, Quaternion_T<T> const& upper )
{
    return {
        unlerp<T, Clamp>( value, lower.w, upper.w ),
        unlerp<T, Clamp>( value, lower.x, upper.x ),
        unlerp<T, Clamp>( value, lower.y, upper.y ),
        unlerp<T, Clamp>( value, lower.z, upper.z ),
    };
}

template<typename T>
constexpr Quaternion_T<T> normalize( Quaternion_T<T> const& num )
{
    return num * (T( 1 ) / num.length());
}

template<typename T>
constexpr Quaternion_T<T> inverse( Quaternion_T<T> const& num )
{
    return { num.w, -num.x, -num.y, -num.z };
}
}

namespace kl
{
template<typename T>
constexpr Vector2<T> abs( Vector2<T> const& vec )
{
    return {
        abs( vec.x ),
        abs( vec.y ),
    };
}

template<typename T>
constexpr Vector2<T> min( Vector2<T> const& vec1, Vector2<T> const& vec2 )
{
    return {
        min( vec1.x, vec2.x ),
        min( vec1.y, vec2.y ),
    };
}

template<typename T>
constexpr Vector2<T> max( Vector2<T> const& vec1, Vector2<T> const& vec2 )
{
    return {
        max( vec1.x, vec2.x ),
        max( vec1.y, vec2.y ),
    };
}

template<typename T>
constexpr Vector2<T> clamp( Vector2<T> const& vec, Vector2<T> const& lower, Vector2<T> const& upper )
{
    return {
        clamp( vec.x, lower.x, upper.x ),
        clamp( vec.y, lower.y, upper.y ),
    };
}

template<typename T, bool Clamp = true>
constexpr Vector2<T> lerp( T value, Vector2<T> const& lower, Vector2<T> const& upper )
{
    return {
        lerp<T, Clamp>( value, lower.x, upper.x ),
        lerp<T, Clamp>( value, lower.y, upper.y ),
    };
}

template<typename T, bool Clamp = true>
constexpr Vector2<T> unlerp( T value, Vector2<T> const& lower, Vector2<T> const& upper )
{
    return {
        unlerp<T, Clamp>( value, lower.x, upper.x ),
        unlerp<T, Clamp>( value, lower.y, upper.y ),
    };
}

template<typename T>
constexpr Vector2<T> normalize( Vector2<T> const& vec )
{
    return vec * (T( 1 ) / vec.length());
}

template<typename T>
constexpr T dot( Vector2<T> const& first, Vector2<T> const& second )
{
    return first.x * second.x + first.y * second.y;
}

template<typename T>
constexpr T angle( Vector2<T> first, Vector2<T> second, bool full )
{
    first = normalize( first );
    second = normalize( second );

    T result = {};
    if ( full )
    {
        result = std::atan2( first.x * second.y - first.y * second.x, first.x * second.x + first.y * second.y );
        result += pi<T>();
    }
    else
    {
        result = acos( dot( first, second ) );
    }
    return result * to_degrees<T>();
}

template<typename T>
constexpr Vector2<T> rotate( Vector2<T> const& vec, T angle )
{
    T sin_a = sin_d( angle );
    T cos_a = cos_d( angle );
    return { cos_a * vec.x - sin_a * vec.y, sin_a * vec.x + cos_a * vec.y };
}

template<typename T>
constexpr Vector2<T> reflect( Vector2<T> const& vec, Vector2<T> point )
{
    point = normalize( point );
    return vec - (point * dot( vec, point ) * T( 2 ));
}
}

namespace kl
{
template<typename T>
constexpr Vector3<T> abs( Vector3<T> const& vec )
{
    return {
        abs( vec.x ),
        abs( vec.y ),
        abs( vec.z ),
    };
}

template<typename T>
constexpr Vector3<T> min( Vector3<T> const& vec1, Vector3<T> const& vec2 )
{
    return {
        min( vec1.x, vec2.x ),
        min( vec1.y, vec2.y ),
        min( vec1.z, vec2.z ),
    };
}

template<typename T>
constexpr Vector3<T> max( Vector3<T> const& vec1, Vector3<T> const& vec2 )
{
    return {
        max( vec1.x, vec2.x ),
        max( vec1.y, vec2.y ),
        max( vec1.z, vec2.z ),
    };
}

template<typename T>
constexpr Vector3<T> clamp( Vector3<T> const& vec, Vector3<T> const& lower, Vector3<T> const& upper )
{
    return {
        clamp( vec.x, lower.x, upper.x ),
        clamp( vec.y, lower.y, upper.y ),
        clamp( vec.z, lower.z, upper.z ),
    };
}

template<typename T, bool Clamp = true>
constexpr Vector3<T> lerp( T value, Vector3<T> const& lower, Vector3<T> const& upper )
{
    return {
        lerp<T, Clamp>( value, lower.x, upper.x ),
        lerp<T, Clamp>( value, lower.y, upper.y ),
        lerp<T, Clamp>( value, lower.z, upper.z ),
    };
}

template<typename T, bool Clamp = true>
constexpr Vector3<T> unlerp( T value, Vector3<T> const& lower, Vector3<T> const& upper )
{
    return {
        unlerp<T, Clamp>( value, lower.x, upper.x ),
        unlerp<T, Clamp>( value, lower.y, upper.y ),
        unlerp<T, Clamp>( value, lower.z, upper.z ),
    };
}

template<typename T>
constexpr Vector3<T> normalize( Vector3<T> const& vec )
{
    return vec * (T( 1 ) / vec.length());
}

template<typename T>
constexpr T dot( Vector3<T> const& first, Vector3<T> const& second )
{
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

template<typename T>
constexpr T angle( Vector3<T> const& first, Vector3<T> const& second )
{
    return acos_d( dot( normalize( first ), normalize( second ) ) );
}

template<typename T>
constexpr Vector3<T> rotate( Vector3<T> const& vec, Vector3<T> const& axis, float angle )
{
    Quaternion_T<T> quat{ axis, angle };
    Quaternion_T<T> inv_quat = inverse( quat );
    return quat * vec * inv_quat;
}

template<typename T>
constexpr Vector3<T> reflect( Vector3<T> const& vec, Vector3<T> normal )
{
    normal = normalize( normal );
    return vec - (normal * dot( vec, normal ) * T( 2 ));
}

template<typename T>
constexpr Vector3<T> refract( Vector3<T> const& vec, Vector3<T> normal, T eta )
{
    normal = normalize( normal );
    float cos_i = -dot( vec, normal );
    float sin_t2 = eta * eta * (1.0f - cos_i * cos_i);
    if ( sin_t2 > 1.0f )
        return {};

    float cos_t = sqrt( 1.0f - sin_t2 );
    return vec * eta + normal * (eta * cos_i - cos_t);
}

template<typename T>
constexpr Vector3<T> cross( Vector3<T> const& first, Vector3<T> const& second )
{
    return {
        first.y * second.z - first.z * second.y,
        first.z * second.x - first.x * second.z,
        first.x * second.y - first.y * second.x,
    };
}
}

namespace kl
{
template<typename T>
constexpr Vector4<T> abs( Vector4<T> const& vec )
{
    return {
        abs( vec.x ),
        abs( vec.y ),
        abs( vec.z ),
        abs( vec.w ),
    };
}

template<typename T>
constexpr Vector4<T> min( Vector4<T> const& vec1, Vector4<T> const& vec2 )
{
    return {
        min( vec1.x, vec2.x ),
        min( vec1.y, vec2.y ),
        min( vec1.z, vec2.z ),
        min( vec1.w, vec2.w ),
    };
}

template<typename T>
constexpr Vector4<T> max( Vector4<T> const& vec1, Vector4<T> const& vec2 )
{
    return {
        max( vec1.x, vec2.x ),
        max( vec1.y, vec2.y ),
        max( vec1.z, vec2.z ),
        max( vec1.w, vec2.w ),
    };
}

template<typename T>
constexpr Vector4<T> clamp( Vector4<T> const& vec, Vector4<T> const& lower, Vector4<T> const& upper )
{
    return {
        clamp( vec.x, lower.x, upper.x ),
        clamp( vec.y, lower.y, upper.y ),
        clamp( vec.z, lower.z, upper.z ),
        clamp( vec.w, lower.w, upper.w ),
    };
}

template<typename T, bool Clamp = true>
constexpr Vector4<T> lerp( T value, Vector4<T> const& lower, Vector4<T> const& upper )
{
    return {
        lerp<T, Clamp>( value, lower.x, upper.x ),
        lerp<T, Clamp>( value, lower.y, upper.y ),
        lerp<T, Clamp>( value, lower.z, upper.z ),
        lerp<T, Clamp>( value, lower.w, upper.w ),
    };
}

template<typename T>
constexpr Vector4<T> normalize( Vector4<T> const& vec )
{
    return vec * (T( 1 ) / vec.length());
}

template<typename T>
constexpr T dot( Vector4<T> const& first, Vector4<T> const& second )
{
    return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
}

template<typename T>
constexpr T angle( Vector4<T> const& first, Vector4<T> const& second )
{
    return acos_d( dot( normalize( first ), normalize( second ) ) );
}
}

namespace kl
{
template<typename T>
constexpr Matrix2x2<T> inverse( Matrix2x2<T> const& mat )
{
    T determinant = mat.determinant();

    Matrix2x2<T> result;
    result[0] = +mat[3];
    result[1] = -mat[1];
    result[2] = -mat[2];
    result[3] = +mat[0];
    return result * determinant;
}

template<typename T>
constexpr Matrix2x2<T> transpose( Matrix2x2<T> const& mat )
{
    Matrix2x2<T> result;
    for ( int y = 0; y < 2; y++ )
    {
        for ( int x = 0; x < 2; x++ )
            result( x, y ) = mat( y, x );
    }
    return result;
}
}

namespace kl
{
template<typename T>
constexpr Matrix3x3<T> inverse( Matrix3x3<T> const& mat )
{
    T determinant = mat.determinant();

    Matrix3x3<T> result;
    result( 0, 0 ) = (mat( 1, 1 ) * mat( 2, 2 ) - mat( 2, 1 ) * mat( 1, 2 )) * determinant;
    result( 0, 1 ) = (mat( 0, 2 ) * mat( 2, 1 ) - mat( 0, 1 ) * mat( 2, 2 )) * determinant;
    result( 0, 2 ) = (mat( 0, 1 ) * mat( 1, 2 ) - mat( 0, 2 ) * mat( 1, 1 )) * determinant;
    result( 1, 0 ) = (mat( 1, 2 ) * mat( 2, 0 ) - mat( 1, 0 ) * mat( 2, 2 )) * determinant;
    result( 1, 1 ) = (mat( 0, 0 ) * mat( 2, 2 ) - mat( 0, 2 ) * mat( 2, 0 )) * determinant;
    result( 1, 2 ) = (mat( 1, 0 ) * mat( 0, 2 ) - mat( 0, 0 ) * mat( 1, 2 )) * determinant;
    result( 2, 0 ) = (mat( 1, 0 ) * mat( 2, 1 ) - mat( 2, 0 ) * mat( 1, 1 )) * determinant;
    result( 2, 1 ) = (mat( 2, 0 ) * mat( 0, 1 ) - mat( 0, 0 ) * mat( 2, 1 )) * determinant;
    result( 2, 2 ) = (mat( 0, 0 ) * mat( 1, 1 ) - mat( 1, 0 ) * mat( 0, 1 )) * determinant;
    return result;
}

template<typename T>
constexpr Matrix3x3<T> transpose( Matrix3x3<T> const& mat )
{
    Matrix3x3<T> result;
    for ( int y = 0; y < 3; y++ )
    {
        for ( int x = 0; x < 3; x++ )
            result( x, y ) = mat( y, x );
    }
    return result;
}
}

namespace kl
{
template<typename T>
constexpr Matrix4x4<T> inverse( Matrix4x4<T> const& mat )
{
    T determinant = mat.determinant();

    T a_2323 = mat( 2, 2 ) * mat( 3, 3 ) - mat( 3, 2 ) * mat( 2, 3 );
    T a_1323 = mat( 1, 2 ) * mat( 3, 3 ) - mat( 3, 2 ) * mat( 1, 3 );
    T a_1223 = mat( 1, 2 ) * mat( 2, 3 ) - mat( 2, 2 ) * mat( 1, 3 );
    T a_0323 = mat( 0, 2 ) * mat( 3, 3 ) - mat( 3, 2 ) * mat( 0, 3 );
    T a_0223 = mat( 0, 2 ) * mat( 2, 3 ) - mat( 2, 2 ) * mat( 0, 3 );
    T a_0123 = mat( 0, 2 ) * mat( 1, 3 ) - mat( 1, 2 ) * mat( 0, 3 );
    T a_2313 = mat( 2, 1 ) * mat( 3, 3 ) - mat( 3, 1 ) * mat( 2, 3 );
    T a_1313 = mat( 1, 1 ) * mat( 3, 3 ) - mat( 3, 1 ) * mat( 1, 3 );
    T a_1213 = mat( 1, 1 ) * mat( 2, 3 ) - mat( 2, 1 ) * mat( 1, 3 );
    T a_2312 = mat( 2, 1 ) * mat( 3, 2 ) - mat( 3, 1 ) * mat( 2, 2 );
    T a_1312 = mat( 1, 1 ) * mat( 3, 2 ) - mat( 3, 1 ) * mat( 1, 2 );
    T a_1212 = mat( 1, 1 ) * mat( 2, 2 ) - mat( 2, 1 ) * mat( 1, 2 );
    T a_0313 = mat( 0, 1 ) * mat( 3, 3 ) - mat( 3, 1 ) * mat( 0, 3 );
    T a_0213 = mat( 0, 1 ) * mat( 2, 3 ) - mat( 2, 1 ) * mat( 0, 3 );
    T a_0312 = mat( 0, 1 ) * mat( 3, 2 ) - mat( 3, 1 ) * mat( 0, 2 );
    T a_0212 = mat( 0, 1 ) * mat( 2, 2 ) - mat( 2, 1 ) * mat( 0, 2 );
    T a_0113 = mat( 0, 1 ) * mat( 1, 3 ) - mat( 1, 1 ) * mat( 0, 3 );
    T a_0112 = mat( 0, 1 ) * mat( 1, 2 ) - mat( 1, 1 ) * mat( 0, 2 );

    Matrix4x4<T> result;
    result( 0, 0 ) = +(mat( 1, 1 ) * a_2323 - mat( 2, 1 ) * a_1323 + mat( 3, 1 ) * a_1223) * determinant;
    result( 1, 0 ) = -(mat( 1, 0 ) * a_2323 - mat( 2, 0 ) * a_1323 + mat( 3, 0 ) * a_1223) * determinant;
    result( 2, 0 ) = +(mat( 1, 0 ) * a_2313 - mat( 2, 0 ) * a_1313 + mat( 3, 0 ) * a_1213) * determinant;
    result( 3, 0 ) = -(mat( 1, 0 ) * a_2312 - mat( 2, 0 ) * a_1312 + mat( 3, 0 ) * a_1212) * determinant;
    result( 0, 1 ) = -(mat( 0, 1 ) * a_2323 - mat( 2, 1 ) * a_0323 + mat( 3, 1 ) * a_0223) * determinant;
    result( 1, 1 ) = +(mat( 0, 0 ) * a_2323 - mat( 2, 0 ) * a_0323 + mat( 3, 0 ) * a_0223) * determinant;
    result( 2, 1 ) = -(mat( 0, 0 ) * a_2313 - mat( 2, 0 ) * a_0313 + mat( 3, 0 ) * a_0213) * determinant;
    result( 3, 1 ) = +(mat( 0, 0 ) * a_2312 - mat( 2, 0 ) * a_0312 + mat( 3, 0 ) * a_0212) * determinant;
    result( 0, 2 ) = +(mat( 0, 1 ) * a_1323 - mat( 1, 1 ) * a_0323 + mat( 3, 1 ) * a_0123) * determinant;
    result( 1, 2 ) = -(mat( 0, 0 ) * a_1323 - mat( 1, 0 ) * a_0323 + mat( 3, 0 ) * a_0123) * determinant;
    result( 2, 2 ) = +(mat( 0, 0 ) * a_1313 - mat( 1, 0 ) * a_0313 + mat( 3, 0 ) * a_0113) * determinant;
    result( 3, 2 ) = -(mat( 0, 0 ) * a_1312 - mat( 1, 0 ) * a_0312 + mat( 3, 0 ) * a_0112) * determinant;
    result( 0, 3 ) = -(mat( 0, 1 ) * a_1223 - mat( 1, 1 ) * a_0223 + mat( 2, 1 ) * a_0123) * determinant;
    result( 1, 3 ) = +(mat( 0, 0 ) * a_1223 - mat( 1, 0 ) * a_0223 + mat( 2, 0 ) * a_0123) * determinant;
    result( 2, 3 ) = -(mat( 0, 0 ) * a_1213 - mat( 1, 0 ) * a_0213 + mat( 2, 0 ) * a_0113) * determinant;
    result( 3, 3 ) = +(mat( 0, 0 ) * a_1212 - mat( 1, 0 ) * a_0212 + mat( 2, 0 ) * a_0112) * determinant;
    return result;
}

template<typename T>
constexpr Matrix4x4<T> transpose( Matrix4x4<T> const& mat )
{
    Matrix4x4<T> result;
    for ( int y = 0; y < 4; y++ )
    {
        for ( int x = 0; x < 4; x++ )
            result( x, y ) = mat( y, x );
    }
    return result;
}
}

namespace kl
{
template<typename T>
constexpr Vector2<T> calc_ndc( Vector2<T> const& position, Vector2<T> const& frame_size )
{
    Vector2<T> result = {
        position.x / frame_size.x,
        (frame_size.y - position.y) / frame_size.y,
    };
    return result * T( 2 ) - Vector2<T>( T( 1 ) );
}

template<typename T>
constexpr Vector2<T> calc_ndc_ar( Vector2<T> const& position, Vector2<T> const& size )
{
    Vector2<T> result = calc_ndc( position, size );
    result.x *= size.x / size.y;
    return result;
}

template<typename T>
constexpr T line_x( Vector2<T> const& a, Vector2<T> const& b, T y )
{
    return T( ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x );
}

template<typename T>
constexpr T line_y( Vector2<T> const& a, Vector2<T> const& b, T x )
{
    return T( ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y );
}

template<typename T>
constexpr Quaternion_T<T> to_quat( Vector3<T> const& euler )
{
    T cr = cos_d( euler.x * T( 0.5 ) );
    T sr = sin_d( euler.x * T( 0.5 ) );
    T cp = cos_d( euler.y * T( 0.5 ) );
    T sp = sin_d( euler.y * T( 0.5 ) );
    T cy = cos_d( euler.z * T( 0.5 ) );
    T sy = sin_d( euler.z * T( 0.5 ) );
    return {
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
    };
}

template<typename T>
constexpr Vector3<T> to_euler( Quaternion_T<T> const& quat )
{
    T       sin_p = T( +2 ) * (quat.w * quat.y - quat.z * quat.x) + T( 0 );
    T sin_r_cos_p = T( +2 ) * (quat.w * quat.x + quat.y * quat.z) + T( 0 );
    T cos_r_cos_p = T( -2 ) * (quat.x * quat.x + quat.y * quat.y) + T( 1 );
    T sin_y_cos_p = T( +2 ) * (quat.w * quat.z + quat.x * quat.y) + T( 0 );
    T cos_y_cos_p = T( -2 ) * (quat.y * quat.y + quat.z * quat.z) + T( 1 );

    Vector3<T> result = {
        std::atan2( sin_r_cos_p, cos_r_cos_p ),
        abs( sin_p ) >= T( 1 ) ? (T) std::copysign( pi<T>() * T( 0.5 ), sin_p ) : (T) asin( sin_p ),
        std::atan2( sin_y_cos_p, cos_y_cos_p ),
    };
    return result * to_degrees<T>();
}

template<typename T>
constexpr Quaternion_T<T> to_quat( Vector3<T> const& original, Vector3<T> const& target )
{
    Vector3<T> axis = normalize( cross( original, target ) );
    T angl = angle( original, target );
    return Quaternion_T{ axis, angl };
}

template<typename T>
constexpr Vector3<T> to_euler( Vector3<T> const& original, Vector3<T> const& target )
{
    return to_euler( to_quat( original, target ) );
}
}
