#include "media/image/color.h"

#include "math/math.h"


// Construct
kl::color::color()
{}

kl::color::color(const byte r, const byte g, const byte b, const byte a)
    : b(b), g(g), r(r), a(a)
{}

// Get
kl::color::operator kl::float3() const
{
    return {
        r * to_float_color,
        g * to_float_color,
        b * to_float_color,
    };
}

kl::color::operator kl::float4() const
{
    return {
        r * to_float_color,
        g * to_float_color,
        b * to_float_color,
        a * to_float_color,
    };
}

// Comapre
bool kl::color::operator==(const color& other) const
{
    return (r == other.r && g == other.g && b == other.b && a == other.a);
}

bool kl::color::operator!=(const color& other) const
{
    return !(*this == other);
}

// Methods
kl::color kl::color::gray() const
{
    const float float_value = (r * 0.299f + g * 0.587f + b * 0.114f);
    const byte gray_value = min(max((int) float_value, 0), 255);
    return { gray_value, gray_value, gray_value, a };
}

kl::color kl::color::inverted() const
{
    return { byte(255 - r), byte(255 - g), byte(255 - b), a };
}

char kl::color::as_ascii() const
{
    static constexpr char ascii_table[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
    static constexpr float conversion = 9.0f / 255.0f;
    return ascii_table[(size_t) (gray().r * conversion)];
}

kl::color kl::color::mix(const color& color, float ratio) const
{
    ratio = min(max(ratio, 0.0f), 1.0f);
    return {
        (byte) ((r * (1.0f - ratio)) + (color.r * ratio)),
        (byte) ((g * (1.0f - ratio)) + (color.g * ratio)),
        (byte) ((b * (1.0f - ratio)) + (color.b * ratio)),
    };
}

kl::color kl::color::mix(const color& color) const
{
    return mix(color, color.a * to_float_color);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const color& object)
{
    stream << "\033[38;2;" << ((int) object.r) << ";" << ((int) object.g) << ";" << ((int) object.b) << "m";
    return stream;
}
