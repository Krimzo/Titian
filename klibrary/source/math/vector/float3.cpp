#include "klibrary.h"


// Construct
kl::Float3::Float3()
{}

kl::Float3::Float3(const float value)
    : x(value), y(value), z(value)
{}

kl::Float3::Float3(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{}

kl::Float3::Float3(const Float2& vec, const float z)
    : x(vec.x), y(vec.y), z(z)
{}

kl::Float3::Float3(float x, const Float2& vec)
    : x(x), y(vec.x), z(vec.y)
{}

// Get
kl::Float3::operator float* ()
{
    return reinterpret_cast<float*>(this);
}

kl::Float3::operator const float* () const
{
    return reinterpret_cast<const float*>(this);
}

kl::Float3::operator kl::Color() const
{
    return {
        (byte) std::clamp(x * TO_BYTE_COLOR, 0.0f, 255.0f),
        (byte) std::clamp(y * TO_BYTE_COLOR, 0.0f, 255.0f),
        (byte) std::clamp(z * TO_BYTE_COLOR, 0.0f, 255.0f),
    };
}

kl::Float3::operator kl::Quaternion() const
{
    return { x, y, z };
}

kl::Float2 kl::Float3::xy() const
{
    return { x, y };
}

float& kl::Float3::operator[](const int index)
{
    return ((float*) this)[index];
}

const float& kl::Float3::operator[](const int index) const
{
    return ((const float*) this)[index];
}

// Compare
bool kl::Float3::operator==(const Float3& other) const
{
    return (x == other.x && y == other.y && z == other.z);
}

bool kl::Float3::operator!=(const Float3& other) const
{
    return !(*this == other);
}

// Math
kl::Float3 kl::Float3::operator+(const Float3& other) const
{
    return { x + other.x, y + other.y, z + other.z };
}

void kl::Float3::operator+=(const Float3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

kl::Float3 kl::Float3::operator-(const Float3& other) const
{
    return { x - other.x, y - other.y, z - other.z };
}

void kl::Float3::operator-=(const Float3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

kl::Float3 kl::Float3::operator*(const float value) const
{
    return { x * value, y * value, z * value, };
}

void kl::Float3::operator*=(const float value)
{
    x *= value;
    y *= value;
    z *= value;
}

kl::Float3 kl::Float3::operator*(const Float3& other) const
{
    return { x * other.x, y * other.y, z * other.z };
}

void kl::Float3::operator*=(const Float3& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
}

kl::Float3 kl::Float3::operator/(const float value) const
{
    return { x / value, y / value, z / value };
}

void kl::Float3::operator/=(const float value)
{
    x /= value;
    y /= value;
    z /= value;
}

kl::Float3 kl::Float3::operator/(const Float3& other) const
{
    return { x / other.x, y / other.y, z / other.z };
}

void kl::Float3::operator/=(const Float3& other)
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
}

// Other
kl::Float3 kl::Float3::operator-() const
{
    return *this * -1.0f;
}

float kl::Float3::length() const
{
    return sqrt(dot(*this, *this));
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Float3& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return stream;
}
