#include "klib.h"


// Construct
kl::Float2x2::Float2x2()
{}

// Get
float& kl::Float2x2::operator[](const int index)
{
    return data[index];
}

const float& kl::Float2x2::operator[](const int index) const
{
    return data[index];
}

float& kl::Float2x2::operator()(const int x, const int y)
{
    return data[y * 2 + x];
}

const float& kl::Float2x2::operator()(const int x, const int y) const
{
    return data[y * 2 + x];
}

// Compare
bool kl::Float2x2::operator==(const Float2x2& other) const
{
    for (int i = 0; i < 4; i++) {
        if (data[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool kl::Float2x2::operator!=(const Float2x2& other) const
{
    return !(*this == other);
}

// Math
kl::Float2x2 kl::Float2x2::operator+(const Float2x2& other) const
{
    Float2x2 result = *this;
    for (int i = 0; i < 4; i++) {
        result[i] += other[i];
    }
    return result;
}

void kl::Float2x2::operator+=(const Float2x2& other)
{
    for (int i = 0; i < 4; i++) {
        data[i] += other[i];
    }
}

kl::Float2x2 kl::Float2x2::operator-(const Float2x2& other) const
{
    Float2x2 result = *this;
    for (int i = 0; i < 4; i++) {
        result[i] -= other[i];
    }
    return result;
}

void kl::Float2x2::operator-=(const Float2x2& other)
{
    for (int i = 0; i < 4; i++) {
        data[i] -= other[i];
    }
}

kl::Float2x2 kl::Float2x2::operator*(const float value) const
{
    Float2x2 result = *this;
    for (int i = 0; i < 4; i++) {
        result[i] *= value;
    }
    return result;
}

void kl::Float2x2::operator*=(const float value)
{
    for (int i = 0; i < 4; i++) {
        data[i] *= value;
    }
}

kl::Float2x2 kl::Float2x2::operator*(const Float2x2& other) const
{
    Float2x2 result = {};
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            result(x, y) = 0.0f;
            for (int i = 0; i < 2; i++) {
                result(x, y) += (*this)(i, y) * other(x, i);
            }
        }
    }
    return result;
}

void kl::Float2x2::operator*=(const Float2x2& other)
{
    *this = (*this * other);
}

kl::Float2 kl::Float2x2::operator*(const Float2& vec) const
{
    Float2 result = {};
    for (int y = 0; y < 2; y++) {
        for (int i = 0; i < 2; i++) {
            result[y] += (*this)(i, y) * vec[i];
        }
    }
    return result;
}

float kl::Float2x2::determinant() const
{
    const float result = data[0] * data[3] - data[1] * data[2];
    return 1.0f / result;
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Float2x2& mat)
{
    stream << format_matrix<2, 2>(mat.data);
    return stream;
}
