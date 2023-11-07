#include "klibrary.h"


// Construct
kl::Complex::Complex()
{}

kl::Complex::Complex(const float r, const float i)
    : r(r), i(i)
{}

kl::Complex::Complex(const float angle)
    : r(cos_deg(angle)), i(sin_deg(angle))
{}

// Get
kl::Complex::operator float* () const
{
    return (float*) this;
}

kl::Complex::operator kl::Float2() const
{
    return *(const Float2*) this;
}

float& kl::Complex::operator[](int index)
{
    return ((float*) this)[index];
}

const float& kl::Complex::operator[](int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::Complex::operator==(const Complex& other) const
{
    return (r == other.r && i == other.i);
}

bool kl::Complex::operator!=(const Complex& other) const
{
    return !(*this == other);
}

// Math
kl::Complex kl::Complex::operator+(const Complex& other) const
{
    return { r + other.r, i + other.i };
}

void kl::Complex::operator+=(const Complex& other)
{
    r += other.r;
    i += other.i;
}

kl::Complex kl::Complex::operator-(const Complex& other) const
{
    return { r - other.r, i - other.i };
}

void kl::Complex::operator-=(const Complex& other)
{
    r -= other.r;
    i -= other.i;
}

kl::Complex kl::Complex::operator*(const float value) const
{
    return { r * value, i * value };
}

void kl::Complex::operator*=(const float value)
{
    r *= value;
    i *= value;
}

kl::Complex kl::Complex::operator*(const Complex& other) const
{
    return {
        r * other.r - i * other.i,
        r * other.i + i * other.r,
    };
}

void kl::Complex::operator*=(const Complex& other)
{
    *this = *this * other;
}

// Other
kl::Complex kl::Complex::operator-() const
{
    return *this * -1.0f;
}

float kl::Complex::length() const
{
    return sqrt(r * r + i * i);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Complex& num)
{
    stream << std::setprecision(2);
    stream << "(" << num.r << " + " << num.i << "i)";
    return stream;
}
