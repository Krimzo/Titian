#include "klibrary.h"


// Construct
kl::Float4::Float4()
{}

kl::Float4::Float4(const float value)
    : x(value), y(value), z(value), w(value)
{}

kl::Float4::Float4(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
{}

kl::Float4::Float4(const Float2& vec, const float z, const float w)
    : x(vec.x), y(vec.y), z(z), w(w)
{}

kl::Float4::Float4(const float x, const Float2& vec, const float w)
    : x(x), y(vec.x), z(vec.y), w(w)
{}

kl::Float4::Float4(const float x, const float y, const Float2& vec)
    : x(x), y(y), z(vec.x), w(vec.y)
{}

kl::Float4::Float4(const Float2& vec1, const Float2& vec2)
    : x(vec1.x), y(vec1.y), z(vec2.x), w(vec2.y)
{}

kl::Float4::Float4(const Float3& vec, const float w)
    : x(vec.x), y(vec.y), z(vec.z), w(w)
{}

kl::Float4::Float4(const float x, const Float3& vec)
    : x(x), y(vec.x), z(vec.y), w(vec.z)
{}

// Get
kl::Float4::operator float* ()
{
    return reinterpret_cast<float*>(this);
}

kl::Float4::operator const float* () const
{
    return reinterpret_cast<const float*>(this);
}

kl::Float4::operator kl::Color() const
{
    return {
        (byte) std::clamp(x * TO_BYTE_COLOR, 0.0f, 255.0f),
        (byte) std::clamp(y * TO_BYTE_COLOR, 0.0f, 255.0f),
        (byte) std::clamp(z * TO_BYTE_COLOR, 0.0f, 255.0f),
        (byte) std::clamp(w * TO_BYTE_COLOR, 0.0f, 255.0f),
    };
}

kl::Float4::operator kl::Quaternion() const
{
    return { w, x, y, z };
}

kl::Float2 kl::Float4::xy() const
{
    return { x, y };
}

kl::Float3 kl::Float4::xyz() const
{
    return { x, y, z };
}

float& kl::Float4::operator[](const int index)
{
    return ((float*) this)[index];
}

const float& kl::Float4::operator[](const int index) const
{
    return ((const float*) this)[index];
}

// Compare
bool kl::Float4::operator==(const Float4& other) const
{
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

bool kl::Float4::operator!=(const Float4& other) const
{
    return !(*this == other);
}

// Math
kl::Float4 kl::Float4::operator+(const Float4& other) const
{
    return { x + other.x, y + other.y, z + other.z, w + other.w };
}

void kl::Float4::operator+=(const Float4& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
}

kl::Float4 kl::Float4::operator-(const Float4& other) const
{
    return { x - other.x, y - other.y, z - other.z, w - other.w };
}

void kl::Float4::operator-=(const Float4& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
}

kl::Float4 kl::Float4::operator*(const float value) const
{
    return { x * value, y * value, z * value, w * value };
}

void kl::Float4::operator*=(const float value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
}

kl::Float4 kl::Float4::operator*(const Float4& other) const
{
    return { x * other.x, y * other.y, z * other.z, w * other.w };
}

void kl::Float4::operator*=(const Float4& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
}

kl::Float4 kl::Float4::operator/(const float value) const
{
    return { x / value, y / value, z / value, w / value };
}

void kl::Float4::operator/=(const float value)
{
    x /= value;
    y /= value;
    z /= value;
    w /= value;
}

kl::Float4 kl::Float4::operator/(const Float4& other) const
{
    return { x / other.x, y / other.y, z / other.z, w / other.w };
}

void kl::Float4::operator/=(const Float4& other)
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
}

// Other
kl::Float4 kl::Float4::operator-() const
{
    return *this * -1.0f;
}

float kl::Float4::length() const
{
    return sqrt(dot(*this, *this));
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Float4& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return stream;
}
