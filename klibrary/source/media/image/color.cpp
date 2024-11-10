#include "klibrary.h"


kl::RGB::RGB()
{}

kl::RGB::RGB( byte r, byte g, byte b, byte a )
    : b( b ), g( g ), r( r ), a( a )
{}

kl::RGB::operator kl::YUV() const
{
    float R = r * to_float_rgb();
    float G = g * to_float_rgb();
    float B = b * to_float_rgb();
    float y = R * 0.299f + G * 0.587f + B * 0.114f;
    float u = R * -0.14713f + G * -0.28886f + B * 0.436f;
    float v = R * 0.615f + G * -0.51499f + B * -0.10001f;
    u = (u + 0.436f) * (1.0f / 0.872f);
    v = (v + 0.615f) * (1.0f / 1.23f);
    return {
        clamp( y, 0.0f, 1.0f ),
        clamp( u, 0.0f, 1.0f ),
        clamp( v, 0.0f, 1.0f ),
    };
}

kl::RGB::operator kl::Float3() const
{
    return Float3{ float( r ), float( g ), float( b ) }  *to_float_rgb();
}

kl::RGB::operator kl::Float4() const
{
    return Float4{ float( r ), float( g ), float( b ), float( a ) }  *to_float_rgb();
}

bool kl::RGB::operator==( RGB other ) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool kl::RGB::operator!=( RGB other ) const
{
    return !(*this == other);
}

kl::RGB kl::RGB::mix( RGB color, float ratio ) const
{
    ratio = clamp( ratio, 0.0f, 1.0f );
    return {
        byte( r * (1.0f - ratio) + color.r * ratio ),
        byte( g * (1.0f - ratio) + color.g * ratio ),
        byte( b * (1.0f - ratio) + color.b * ratio ),
    };
}

kl::RGB kl::RGB::mix( RGB color ) const
{
    return mix( color, color.a * to_float_rgb() );
}

kl::RGB kl::RGB::gray() const
{
    float light = YUV( *this ).y * 255.0f;
    byte result = (byte) clamp( light, 0.0f, 255.0f );
    return { result, result, result, a };
}

kl::RGB kl::RGB::inverted() const
{
    return { byte( 255 - r ), byte( 255 - g ), byte( 255 - b ), a };
}

char kl::RGB::ascii() const
{
    return YUV( *this ).ascii();
}

kl::YUV::YUV()
{}

kl::YUV::YUV( float y, float u, float v )
    : y( y ), u( u ), v( v )
{}

kl::YUV::operator kl::RGB() const
{
    float Y = y;
    float U = u * 0.872f - 0.436f;
    float V = v * 1.23f - 0.615f;
    float r = Y + 1.13983f * V;
    float g = Y - 0.39465f * U - 0.58060f * V;
    float b = Y + 2.03211f * U;
    return {
        (byte) clamp( r * 255.0f, 0.0f, 255.0f ),
        (byte) clamp( g * 255.0f, 0.0f, 255.0f ),
        (byte) clamp( b * 255.0f, 0.0f, 255.0f ),
    };
}

kl::YUV::operator kl::Vector3<float>() const
{
    return { y, u, v };
}

bool kl::YUV::operator==( YUV const& other ) const
{
    return y == other.y && u == other.u && v == other.v;
}

bool kl::YUV::operator!=( YUV const& other ) const
{
    return !(*this == other);
}

char kl::YUV::ascii() const
{
    static constexpr char ascii_table[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
    return ascii_table[int( y * 9.0f )];
}

std::ostream& kl::operator<<( std::ostream& stream, RGB col )
{
    stream << "\033[38;2;" << int( col.r ) << ";" << int( col.g ) << ";" << int( col.b ) << "m";
    return stream;
}

std::ostream& kl::operator<<( std::ostream& stream, YUV const& col )
{
    stream << std::setprecision( 2 );
    stream << '(' << col.y << ", " << col.u << ", " << col.v << ')';
    return stream;
}
