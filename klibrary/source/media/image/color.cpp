#include "klibrary.h"


// Construct
kl::Color::Color()
{}

kl::Color::Color(const byte r, const byte g, const byte b, const byte a)
    : b(b), g(g), r(r), a(a)
{}

// Get
kl::Color::operator kl::Float3() const
{
    return {
        static_cast<float>(r * TO_FLOAT_COLOR),
        static_cast<float>(g * TO_FLOAT_COLOR),
        static_cast<float>(b * TO_FLOAT_COLOR),
    };
}

kl::Color::operator kl::Float4() const
{
    return {
        static_cast<float>(r * TO_FLOAT_COLOR),
        static_cast<float>(g * TO_FLOAT_COLOR),
        static_cast<float>(b * TO_FLOAT_COLOR),
        static_cast<float>(a * TO_FLOAT_COLOR),
    };
}

// Comapre
bool kl::Color::operator==(const Color& other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool kl::Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

// Methods
kl::Color kl::Color::gray() const
{
    const float float_value = r * 0.299f + g * 0.587f + b * 0.114f;
    const byte gray_value = std::clamp<int>((int) float_value, 0, 255);
    return { gray_value, gray_value, gray_value, a };
}

kl::Color kl::Color::inverted() const
{
    return { byte(255 - r), byte(255 - g), byte(255 - b), a };
}

char kl::Color::as_ascii() const
{
    static constexpr char ascii_table[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
    static constexpr float conversion = 9.0f / 255.0f;
    return ascii_table[(size_t) (gray().r * conversion)];
}

kl::Color kl::Color::mix(const Color& color, float ratio) const
{
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    return {
        (byte) (r * (1.0f - ratio) + color.r * ratio),
        (byte) (g * (1.0f - ratio) + color.g * ratio),
        (byte) (b * (1.0f - ratio) + color.b * ratio),
    };
}

kl::Color kl::Color::mix(const Color& color) const
{
    return mix(color, float(color.a * TO_FLOAT_COLOR));
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Color& object)
{
    stream << "\033[38;2;" << ((int) object.r) << ";" << ((int) object.g) << ";" << ((int) object.b) << "m";
    return stream;
}
