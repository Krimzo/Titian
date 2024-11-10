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

    RGB();
    RGB( byte r, byte g, byte b, byte a = 255 );

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

    YUV();
    YUV( float y, float u, float v );

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
std::ostream& operator<<( std::ostream& stream, YUV const& col );
}

namespace kl::colors
{
inline RGB CONSOLE = { 205, 205, 205 };
inline RGB   BLACK = { 0, 0, 0 };
inline RGB   WHITE = { 255, 255, 255 };
inline RGB    GRAY = { 50, 50, 50 };
inline RGB     RED = { 255, 0, 0 };
inline RGB   GREEN = { 0, 255, 0 };
inline RGB    BLUE = { 0, 0, 255 };
inline RGB    CYAN = { 30, 180, 170 };
inline RGB  PURPLE = { 220, 0, 220 };
inline RGB  YELLOW = { 220, 220, 0 };
inline RGB  ORANGE = { 255, 140, 0 };
inline RGB MAGENTA = { 155, 0, 155 };
inline RGB CRIMSON = { 100, 0, 0 };
inline RGB   WHEAT = { 245, 220, 180 };
inline RGB     SKY = { 190, 245, 255 };
}
