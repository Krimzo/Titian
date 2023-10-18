#include "klib.h"


// Construct
kl::Float2::Float2()
{}

kl::Float2::Float2(const float value)
    : x(value), y(value)
{}

kl::Float2::Float2(const float x, const float y)
    : x(x), y(y)
{}

// Get
kl::Float2::operator float* ()
{
    return reinterpret_cast<float*>(this);
}

kl::Float2::operator const float* () const
{
    return reinterpret_cast<const float*>(this);
}

kl::Float2::operator kl::Int2() const
{
    return { (int) x, (int) y };
}

kl::Float2::operator kl::Complex() const
{
    return *(const Complex*) this;
}

float& kl::Float2::operator[](const int index)
{
    return ((float*) this)[index];
}

const float& kl::Float2::operator[](const int index) const
{
    return ((const float*) this)[index];
}

// Compare
bool kl::Float2::operator==(const Float2& other) const
{
    return (x == other.x && y == other.y);
}

bool kl::Float2::operator!=(const Float2& other) const
{
    return !(*this == other);
}

// Math
kl::Float2 kl::Float2::operator+(const Float2& other) const
{
    return { x + other.x, y + other.y };
}

void kl::Float2::operator+=(const Float2& other)
{
    x += other.x;
    y += other.y;
}

kl::Float2 kl::Float2::operator-(const Float2& other) const
{
    return { x - other.x, y - other.y };
}

void kl::Float2::operator-=(const Float2& other)
{
    x -= other.x;
    y -= other.y;
}

kl::Float2 kl::Float2::operator*(const float value) const
{
    return { x * value, y * value };
}

void kl::Float2::operator*=(const float value)
{
    x *= value;
    y *= value;
}

kl::Float2 kl::Float2::operator*(const Float2& other) const
{
    return { x * other.x, y * other.y };
}

void kl::Float2::operator*=(const Float2& other)
{
    x *= other.x;
    y *= other.y;
}

kl::Float2 kl::Float2::operator/(const float value) const
{
    return { x / value, y / value };
}

void kl::Float2::operator/=(const float value)
{
    x /= value;
    y /= value;
}

kl::Float2 kl::Float2::operator/(const Float2& other) const
{
    return { x / other.x, y / other.y };
}

void kl::Float2::operator/=(const Float2& other)
{
    x /= other.x;
    y /= other.y;
}

// Other
kl::Float2 kl::Float2::operator-() const
{
    return *this * -1.0f;
}

float kl::Float2::length() const
{
    return sqrt(dot(*this, *this));
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Float2& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}
