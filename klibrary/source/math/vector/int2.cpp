#include "klibrary.h"


// Construct
kl::Int2::Int2()
{}

kl::Int2::Int2(const int value)
    : x(value), y(value)
{}

kl::Int2::Int2(const int x, const int y)
    : x(x), y(y)
{}

// Get
kl::Int2::operator int* ()
{
    return reinterpret_cast<int*>(this);
}

kl::Int2::operator const int* () const
{
    return reinterpret_cast<const int*>(this);
}

kl::Int2::operator kl::Float2() const
{
    return { (float) x, (float) y };
}

int& kl::Int2::operator[](const int index)
{
    return ((int*) this)[index];
}

const int& kl::Int2::operator[](const int index) const
{
    return ((const int*) this)[index];
}

// Compare
bool kl::Int2::operator==(const Int2& other) const
{
    return (x == other.x && y == other.y);
}

bool kl::Int2::operator!=(const Int2& other) const
{
    return !(*this == other);
}

// Math
kl::Int2 kl::Int2::operator+(const Int2& other) const
{
    return { x + other.x, y + other.y };
}

void kl::Int2::operator+=(const Int2& other)
{
    x += other.x;
    y += other.y;
}

kl::Int2 kl::Int2::operator-(const Int2& other) const
{
    return { x - other.x, y - other.y };
}

void kl::Int2::operator-=(const Int2& other)
{
    x -= other.x;
    y -= other.y;
}

kl::Int2 kl::Int2::operator*(const int value) const
{
    return { x * value, y * value };
}

void kl::Int2::operator*=(const int value)
{
    x *= value;
    y *= value;
}

kl::Int2 kl::Int2::operator*(const Int2& other) const
{
    return { x * other.x, y * other.y };
}

void kl::Int2::operator*=(const Int2& other)
{
    x *= other.x;
    y *= other.y;
}

kl::Int2 kl::Int2::operator/(int value) const
{
    return { x / value, y / value };
}

void kl::Int2::operator/=(int value)
{
    x /= value;
    y /= value;
}

kl::Int2 kl::Int2::operator/(const Int2& other) const
{
    return { x / other.x, y / other.y };
}

void kl::Int2::operator/=(const Int2& other)
{
    x /= other.x;
    y /= other.y;
}

// Other
kl::Int2 kl::Int2::operator-() const
{
    return *this * -1;
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Int2& vec)
{
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}
