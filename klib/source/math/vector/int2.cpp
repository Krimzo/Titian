#include "math/vector/int2.h"

#include "math/vector/float2.h"


// Construct
kl::int2::int2()
{}

kl::int2::int2(const int value)
    : x(value), y(value)
{}

kl::int2::int2(const int x, const int y)
    : x(x), y(y)
{}

// Get
kl::int2::operator int* () const
{
    return (int*) this;
}

kl::int2::operator kl::float2() const
{
    return { (float) x, (float) y };
}

int& kl::int2::operator[](const int index)
{
    return ((int*) this)[index];
}

const int& kl::int2::operator[](const int index) const
{
    return ((int*) this)[index];
}

// Compare
bool kl::int2::operator==(const int2& other) const
{
    return (x == other.x && y == other.y);
}

bool kl::int2::operator!=(const int2& other) const
{
    return !(*this == other);
}

// Math
kl::int2 kl::int2::operator+(const int2& other) const
{
    return { x + other.x, y + other.y };
}

void kl::int2::operator+=(const int2& other)
{
    x += other.x;
    y += other.y;
}

kl::int2 kl::int2::operator-(const int2& other) const
{
    return { x - other.x, y - other.y };
}

void kl::int2::operator-=(const int2& other)
{
    x -= other.x;
    y -= other.y;
}

kl::int2 kl::int2::operator*(const int value) const
{
    return { x * value, y * value };
}

void kl::int2::operator*=(const int value)
{
    x *= value;
    y *= value;
}

kl::int2 kl::int2::operator*(const int2& other) const
{
    return { x * other.x, y * other.y };
}

void kl::int2::operator*=(const int2& other)
{
    x *= other.x;
    y *= other.y;
}

kl::int2 kl::int2::operator/(int value) const
{
    return { x / value, y / value };
}

void kl::int2::operator/=(int value)
{
    x /= value;
    y /= value;
}

kl::int2 kl::int2::operator/(const int2& other) const
{
    return { x / other.x, y / other.y };
}

void kl::int2::operator/=(const int2& other)
{
    x /= other.x;
    y /= other.y;
}

// Other
kl::int2 kl::int2::operator-() const
{
    return *this * -1;
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const int2& vec)
{
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}
