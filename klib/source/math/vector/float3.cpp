#include "math/vector/float3.h"


// Construct
kl::float3::float3()
{}

kl::float3::float3(const float value)
    : x(value), y(value), z(value)
{}

kl::float3::float3(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{}

kl::float3::float3(const float2& vec, const float z)
    : x(vec.x), y(vec.y), z(z)
{}

kl::float3::float3(float x, const float2& vec)
    : x(x), y(vec.x), z(vec.y)
{}

// Get
kl::float3::operator float* () const
{
    return (float*) this;
}

kl::float3::operator kl::color() const
{
    return {
        (byte) min(max((int) (x * math::to_byte_color), 0), 255),
        (byte) min(max((int) (y * math::to_byte_color), 0), 255),
        (byte) min(max((int) (z * math::to_byte_color), 0), 255),
    };
}

kl::float2 kl::float3::xy() const
{
    return { x, y };
}

float& kl::float3::operator[](int index)
{
    return ((float*) this)[index];
}

const float& kl::float3::operator[](int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::float3::operator==(const float3& other) const
{
    return (x == other.x && y == other.y && z == other.z);
}

bool kl::float3::operator!=(const float3& other) const
{
    return !(*this == other);
}

// Math
kl::float3 kl::float3::operator+(const float3& other) const
{
    return { x + other.x, y + other.y, z + other.z };
}

void kl::float3::operator+=(const float3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

kl::float3 kl::float3::operator-(const float3& other) const
{
    return { x - other.x, y - other.y, z - other.z };
}

void kl::float3::operator-=(const float3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

kl::float3 kl::float3::operator*(const float value) const
{
    return { x * value, y * value, z * value, };
}

void kl::float3::operator*=(const float value)
{
    x *= value;
    y *= value;
    z *= value;
}

float kl::float3::operator*(const float3& other) const
{
    return (x * other.x + y * other.y + z * other.z);
}

float kl::float3::length() const
{
    return sqrt(*this * *this);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const float3& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return stream;
}
