#pragma once

#include "apis/apis.h"


namespace kl
{
struct RGB;
struct YUV;
template<typename T> struct Vector3;
template<typename T> struct Vector4;
}

namespace kl
{
struct RGB
{
    byte b = 0;
    byte g = 0;
    byte r = 0;
    byte a = 255;

    constexpr RGB()
    {}

    constexpr RGB( byte r, byte g, byte b, byte a = 255 )
        : b( b ), g( g ), r( r ), a( a )
    {}

    operator YUV() const;
    operator Vector3<float>() const;
    operator Vector4<float>() const;

    bool operator==( RGB other ) const;
    bool operator!=( RGB other ) const;

    RGB mix( RGB color, float ratio ) const;
    RGB mix( RGB color ) const;

    RGB gray() const;
    RGB inverted() const;
    char ascii() const;
};
}

namespace kl
{
struct YUV
{
    float y = 0.0f;
    float u = 0.0f;
    float v = 0.0f;

    constexpr YUV()
    {}

    constexpr YUV( float y, float u, float v )
        : y( y ), u( u ), v( v )
    {}

    operator RGB() const;
    operator Vector3<float>() const;

    bool operator==( YUV const& other ) const;
    bool operator!=( YUV const& other ) const;

    char ascii() const;
};
}

namespace kl
{
std::ostream& operator<<( std::ostream& stream, RGB col );
std::wostream& operator<<( std::wostream& stream, RGB col );
std::ostream& operator<<( std::ostream& stream, YUV const& col );
std::wostream& operator<<( std::wostream& stream, YUV const& col );
}

namespace kl::colors
{
inline constexpr RGB CONSOLE = { 205, 205, 205 };
inline constexpr RGB   BLACK = { 0, 0, 0 };
inline constexpr RGB   WHITE = { 255, 255, 255 };
inline constexpr RGB    GRAY = { 50, 50, 50 };
inline constexpr RGB     RED = { 255, 0, 0 };
inline constexpr RGB   GREEN = { 0, 255, 0 };
inline constexpr RGB    BLUE = { 0, 0, 255 };
inline constexpr RGB    CYAN = { 30, 180, 170 };
inline constexpr RGB  PURPLE = { 220, 0, 220 };
inline constexpr RGB  YELLOW = { 220, 220, 0 };
inline constexpr RGB  ORANGE = { 255, 140, 0 };
inline constexpr RGB MAGENTA = { 155, 0, 155 };
inline constexpr RGB CRIMSON = { 100, 0, 0 };
inline constexpr RGB   WHEAT = { 245, 220, 180 };
inline constexpr RGB     SKY = { 190, 245, 255 };
}
