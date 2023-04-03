#include "math/imaginary/complex.h"

#include "math/math.h"


// Construct
kl::complex::complex()
{}

kl::complex::complex(const float r, const float i)
    : r(r), i(i)
{}

kl::complex::complex(const float angle)
    : r(cos_deg(angle)), i(sin_deg(angle))
{}

// Get
kl::complex::operator float* () const
{
    return (float*) this;
}

kl::complex::operator kl::float2() const
{
    return *(const float2*) this;
}

float& kl::complex::operator[](int index)
{
    return ((float*) this)[index];
}

const float& kl::complex::operator[](int index) const
{
    return ((float*) this)[index];
}

// Compare
bool kl::complex::operator==(const complex& other) const
{
    return (r == other.r && i == other.i);
}

bool kl::complex::operator!=(const complex& other) const
{
    return !(*this == other);
}

// Math
kl::complex kl::complex::operator+(const complex& other) const
{
    return { r + other.r, i + other.i };
}

void kl::complex::operator+=(const complex& other)
{
    r += other.r;
    i += other.i;
}

kl::complex kl::complex::operator-(const complex& other) const
{
    return { r - other.r, i - other.i };
}

void kl::complex::operator-=(const complex& other)
{
    r -= other.r;
    i -= other.i;
}

kl::complex kl::complex::operator*(const float value) const
{
    return { r * value, i * value };
}

void kl::complex::operator*=(const float value)
{
    r *= value;
    i *= value;
}

kl::complex kl::complex::operator*(const complex& other) const
{
    return {
        r * other.r - i * other.i,
        r * other.i + i * other.r,
    };
}

void kl::complex::operator*=(const complex& other)
{
    *this = *this * other;
}

// Other
kl::complex kl::complex::operator-() const
{
    return *this * -1.0f;
}

float kl::complex::length() const
{
    return sqrt(r * r + i * i);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const complex& num)
{
    stream << std::setprecision(2);
    stream << "(" << num.r << " + " << num.i << "i)";
    return stream;
}
