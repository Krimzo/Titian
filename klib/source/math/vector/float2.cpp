#include "math/vector/float2.h"

#include "math/math.h"


// Construct
kl::float2::float2()
{}

kl::float2::float2(const float value)
    : x(value), y(value)
{}

kl::float2::float2(const float x, const float y)
    : x(x), y(y)
{}

// Get
kl::float2::operator float* () const
{
    return (float*) this;
}

kl::float2::operator kl::int2() const
{
    return { (int) x, (int) y };
}

float& kl::float2::operator[](int index)
{
    return ((float*) this)[index];
}

const float& kl::float2::operator[](int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::float2::operator==(const float2& other) const
{
    return (x == other.x && y == other.y);
}

bool kl::float2::operator!=(const float2& other) const
{
    return !(*this == other);
}

// Math
kl::float2 kl::float2::operator+(const float2& other) const
{
    return { x + other.x, y + other.y };
}

void kl::float2::operator+=(const float2& other)
{
    x += other.x;
    y += other.y;
}

kl::float2 kl::float2::operator-(const float2& other) const
{
    return { x - other.x, y - other.y };
}

void kl::float2::operator-=(const float2& other)
{
    x -= other.x;
    y -= other.y;
}

kl::float2 kl::float2::operator*(const float value) const
{
    return { x * value, y * value };
}

void kl::float2::operator*=(const float value)
{
    x *= value;
    y *= value;
}

kl::float2 kl::float2::operator*(const float2& other) const
{
    return { x * other.x, y * other.y };
}

void kl::float2::operator*=(const float2& other)
{
    x *= other.x;
    y *= other.y;
}

kl::float2 kl::float2::operator/(const float value) const
{
    return { x / value, y / value };
}

void kl::float2::operator/=(const float value)
{
    x /= value;
    y /= value;
}

kl::float2 kl::float2::operator/(const float2& other) const
{
    return { x / other.x, y / other.y };
}

void kl::float2::operator/=(const float2& other)
{
    x /= other.x;
    y /= other.y;
}

kl::float2 kl::float2::operator-() const
{
    return *this * -1.0f;
}

float kl::float2::length() const
{
    return sqrt(dot(*this, *this));
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const float2& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}
