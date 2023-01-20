#include "media/color.h"

#include "math/math.h"


kl::color::color()
    : b(0), g(0), r(0), a(255)
{}

kl::color::color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    : b(b), g(g), r(r), a(a)
{}

bool kl::color::operator==(const color& obj) const
{
    return r == obj.r && g == obj.g && b == obj.b && a == obj.a;
}

bool kl::color::operator!=(const color& obj) const
{
    return !(*this == obj);
}

kl::color kl::color::gray() const
{
    const uint8_t gray_value = uint8_t(r * 0.3f + g * 0.585f + b * 0.115f);
    return { gray_value, gray_value, gray_value, a };
}

kl::color kl::color::inverted() const
{
    return { uint8_t(255 - r), uint8_t(255 - g), uint8_t(255 - b), a };
}

char kl::color::as_ascii() const
{
    static constexpr char ascii_table[10] = {'@', '%', '#', 'x', '+', '=', ':', '-', '.', ' '};
    static constexpr float conversion = 9.0f / 255.0f;
    return ascii_table[size_t(gray().r * conversion)];
}

kl::color kl::color::mix(const color& color, float ratio) const
{
    ratio = math::minmax(ratio, 0, 1);
    return {
        uint8_t((r * (1.0f - ratio)) + (color.r * ratio)),
        uint8_t((g * (1.0f - ratio)) + (color.g * ratio)),
        uint8_t((b * (1.0f - ratio)) + (color.b * ratio))
    };
}

kl::color kl::color::mix(const color& color) const
{
    return mix(color, to_float_color(color.a));
}

std::ostream& kl::operator<<(std::ostream& stream, const color& object)
{
    stream << "\033[38;2;" << int(object.r) << ";" << int(object.g) << ";" << int(object.b) <<
        "m";
    return stream;
}
