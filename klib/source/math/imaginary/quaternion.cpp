#include "klib.h"


// Construct
kl::quaternion::quaternion()
    : w(1.0f)
{}

kl::quaternion::quaternion(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{}

kl::quaternion::quaternion(const float w, const float x, const float y, const float z)
    : w(w), x(x), y(y), z(z)
{}

kl::quaternion::quaternion(const float3& axis, const float angle)
    : w(cos_deg(angle * 0.5f))
{
    ((float3&) x) = normalize(axis) * sin_deg(angle * 0.5f);
}

// Get
kl::quaternion::operator float* () const
{
    return (float*) this;
}

kl::quaternion::operator kl::float3() const
{
    return { x, y, z };
}

kl::quaternion::operator kl::float4() const
{
    return { x, y, z, w };
}

float& kl::quaternion::operator[](const int index)
{
    return ((float*) this)[index];
}

const float& kl::quaternion::operator[](const int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::quaternion::operator==(const quaternion& other) const
{
    return (w == other.w && x == other.x && y == other.y && z == other.z);
}

bool kl::quaternion::operator!=(const quaternion& other) const
{
    return !(*this == other);
}

// Math
kl::quaternion kl::quaternion::operator+(const quaternion& other) const
{
    return { w + other.w, x + other.x, y + other.y, z + other.z };
}

void kl::quaternion::operator+=(const quaternion& other)
{
    w += other.w;
    x += other.x;
    y += other.y;
    z += other.z;
}

kl::quaternion kl::quaternion::operator-(const quaternion& other) const
{
    return { w - other.w, x - other.x, y - other.y, z - other.z };
}

void kl::quaternion::operator-=(const quaternion& other)
{
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

kl::quaternion kl::quaternion::operator*(const float value) const
{
    return { w * value, x * value, y * value, z * value };
}

void kl::quaternion::operator*=(const float value)
{
    w *= value;
    x *= value;
    y *= value;
    z *= value;
}

kl::quaternion kl::quaternion::operator*(const quaternion& other) const
{
    return {
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
    };
}

void kl::quaternion::operator*=(const quaternion& other)
{
    *this = *this * other;
}

// Other
kl::quaternion kl::quaternion::operator-() const
{
    return *this * -1.0f;
}

float kl::quaternion::length() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const quaternion& num)
{
    stream << std::setprecision(2);
    stream << "(" << num.w << " + " << num.x << "i + " << num.y << "j + " << num.z << "k)";
    return stream;
}
