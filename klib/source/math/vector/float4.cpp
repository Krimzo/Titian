#include "math/vector/float4.h"


// Construct
kl::float4::float4()
{}

kl::float4::float4(const float value)
    : x(value), y(value), z(value), w(value)
{}

kl::float4::float4(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
{}

kl::float4::float4(const float2& vec, const float z, const float w)
    : x(vec.x), y(vec.y), z(z), w(w)
{}

kl::float4::float4(const float x, const float2& vec, const float w)
    : x(x), y(vec.x), z(vec.y), w(w)
{}

kl::float4::float4(const float x, const float y, const float2& vec)
    : x(x), y(y), z(vec.x), w(vec.y)
{}

kl::float4::float4(const float2& vec1, const float2& vec2)
    : x(vec1.x), y(vec1.y), z(vec2.x), w(vec2.y)
{}

kl::float4::float4(const float3& vec, float w)
    : x(vec.x), y(vec.y), z(vec.z), w(w)
{}

kl::float4::float4(float x, const float3& vec)
    : x(x), y(vec.x), z(vec.y), w(vec.z)
{}

// Get
kl::float4::operator float* () const
{
    return (float*) this;
}

kl::float4::operator kl::color() const
{
    return {
        (byte) min(max((int) (x * math::to_byte_color), 0), 255),
        (byte) min(max((int) (y * math::to_byte_color), 0), 255),
        (byte) min(max((int) (z * math::to_byte_color), 0), 255),
        (byte) min(max((int) (w * math::to_byte_color), 0), 255),
    };
}

kl::float2 kl::float4::xy() const
{
    return { x, y };
}

kl::float3 kl::float4::xyz() const
{
    return { x, y, z };
}

float& kl::float4::operator[](int index)
{
    return ((float*) this)[index];
}

const float& kl::float4::operator[](int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::float4::operator==(const float4& other) const
{
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

bool kl::float4::operator!=(const float4& other) const
{
    return !(*this == other);
}

// Math
kl::float4 kl::float4::operator+(const float4& other) const
{
    return { x + other.x, y + other.y, z + other.z, w + other.w };
}

void kl::float4::operator+=(const float4& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
}

kl::float4 kl::float4::operator-(const float4& other) const
{
    return { x - other.x, y - other.y, z - other.z, w - other.w };
}

void kl::float4::operator-=(const float4& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
}

kl::float4 kl::float4::operator*(const float value) const
{
    return { x * value, y * value, z * value, w * value };
}

void kl::float4::operator*=(const float value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
}

float kl::float4::operator*(const float4& other) const
{
    return (x * other.x + y * other.y + z * other.z + w * other.w);
}

float kl::float4::length() const
{
    return sqrt(*this * *this);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const float4& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return stream;
}
