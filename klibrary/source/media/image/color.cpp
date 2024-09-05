#include "klibrary.h"


kl::Color::Color()
{}

kl::Color::Color(const byte r, const byte g, const byte b, const byte a)
    : b(b), g(g), r(r), a(a)
{}

kl::Color::operator kl::Float3() const
{
    return Float3{ float(r), float(g), float(b) } * to_float_rgb();
}

kl::Color::operator kl::Float4() const
{
	return Float4{ float(r), float(g), float(b), float(a) } * to_float_rgb();
}

bool kl::Color::operator==(const Color& other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool kl::Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

kl::Color kl::Color::gray() const
{
    const double light_value = r * 0.299 + g * 0.587 + b * 0.114;
    const byte gray_value = (byte) clamp(int(light_value), 0, 255);
    return { gray_value, gray_value, gray_value, a };
}

kl::Color kl::Color::inverted() const
{
    return { byte(255 - r), byte(255 - g), byte(255 - b), a };
}

char kl::Color::as_ascii() const
{
    static constexpr char ascii_table[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
    return ascii_table[int(gray().r * (9.0 / 255.0))];
}

kl::Color kl::Color::mix(const Color& color, float ratio) const
{
    ratio = clamp(ratio, 0.0f, 1.0f);
    return {
        byte(r * (1.0f - ratio) + color.r * ratio),
        byte(g * (1.0f - ratio) + color.g * ratio),
        byte(b * (1.0f - ratio) + color.b * ratio),
    };
}

kl::Color kl::Color::mix(const Color& color) const
{
    return mix(color, color.a * to_float_rgb());
}

std::ostream& kl::operator<<(std::ostream& stream, const Color& object)
{
    stream << "\033[38;2;" << ((int) object.r) << ";" << ((int) object.g) << ";" << ((int) object.b) << "m";
    return stream;
}
