#include "klibrary.h"


// Construct
kl::Quaternion::Quaternion()
    : w(1.0f)
{}

kl::Quaternion::Quaternion(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{}

kl::Quaternion::Quaternion(const float w, const float x, const float y, const float z)
    : w(w), x(x), y(y), z(z)
{}

kl::Quaternion::Quaternion(const Float3& axis, const float angle)
    : w(cos_deg(angle * 0.5f))
{
    ((Float3&) x) = normalize(axis) * sin_deg(angle * 0.5f);
}

// Get
kl::Quaternion::operator float* () const
{
    return (float*) this;
}

kl::Quaternion::operator kl::Float3() const
{
    return { x, y, z };
}

kl::Quaternion::operator kl::Float4() const
{
    return { x, y, z, w };
}

float& kl::Quaternion::operator[](const int index)
{
    return ((float*) this)[index];
}

const float& kl::Quaternion::operator[](const int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::Quaternion::operator==(const Quaternion& other) const
{
    return (w == other.w && x == other.x && y == other.y && z == other.z);
}

bool kl::Quaternion::operator!=(const Quaternion& other) const
{
    return !(*this == other);
}

// Math
kl::Quaternion kl::Quaternion::operator+(const Quaternion& other) const
{
    return { w + other.w, x + other.x, y + other.y, z + other.z };
}

void kl::Quaternion::operator+=(const Quaternion& other)
{
    w += other.w;
    x += other.x;
    y += other.y;
    z += other.z;
}

kl::Quaternion kl::Quaternion::operator-(const Quaternion& other) const
{
    return { w - other.w, x - other.x, y - other.y, z - other.z };
}

void kl::Quaternion::operator-=(const Quaternion& other)
{
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

kl::Quaternion kl::Quaternion::operator*(const float value) const
{
    return { w * value, x * value, y * value, z * value };
}

void kl::Quaternion::operator*=(const float value)
{
    w *= value;
    x *= value;
    y *= value;
    z *= value;
}

kl::Quaternion kl::Quaternion::operator*(const Quaternion& other) const
{
    return {
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
    };
}

void kl::Quaternion::operator*=(const Quaternion& other)
{
    *this = *this * other;
}

// Other
kl::Quaternion kl::Quaternion::operator-() const
{
    return *this * -1.0f;
}

float kl::Quaternion::length() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Quaternion& num)
{
    stream << std::setprecision(2);
    stream << "(" << num.w << " + " << num.x << "i + " << num.y << "j + " << num.z << "k)";
    return stream;
}
