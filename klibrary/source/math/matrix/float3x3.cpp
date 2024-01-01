#include "klibrary.h"


// Construct
kl::Float3x3::Float3x3()
{}

// Get
float& kl::Float3x3::operator[](const int index)
{
    return data[index];
}

const float& kl::Float3x3::operator[](const int index) const
{
    return data[index];
}

float& kl::Float3x3::operator()(const int x, const int y)
{
    return data[y * 3 + x];
}

const float& kl::Float3x3::operator()(const int x, const int y) const
{
    return data[y * 3 + x];
}

// Compare
bool kl::Float3x3::operator==(const Float3x3& other) const
{
    for (int i = 0; i < 9; i++) {
        if (data[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool kl::Float3x3::operator!=(const Float3x3& other) const
{
    return !(*this == other);
}

// Math
kl::Float3x3 kl::Float3x3::operator+(const Float3x3& other) const
{
    Float3x3 result = *this;
    for (int i = 0; i < 9; i++) {
        result[i] += other[i];
    }
    return result;
}

void kl::Float3x3::operator+=(const Float3x3& other)
{
    for (int i = 0; i < 9; i++) {
        data[i] += other[i];
    }
}

kl::Float3x3 kl::Float3x3::operator-(const Float3x3& other) const
{
    Float3x3 result = *this;
    for (int i = 0; i < 9; i++) {
        result[i] -= other[i];
    }
    return result;
}

void kl::Float3x3::operator-=(const Float3x3& other)
{
    for (int i = 0; i < 9; i++) {
        data[i] -= other[i];
    }
}

kl::Float3x3 kl::Float3x3::operator*(const float value) const
{
    Float3x3 result = *this;
    for (int i = 0; i < 9; i++) {
        result[i] *= value;
    }
    return result;
}

void kl::Float3x3::operator*=(const float value)
{
    for (int i = 0; i < 9; i++) {
        data[i] *= value;
    }
}

kl::Float3x3 kl::Float3x3::operator*(const Float3x3& other) const
{
    Float3x3 result = {};
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            result(x, y) = 0.0f;
            for (int i = 0; i < 3; i++) {
                result(x, y) += (*this)(i, y) * other(x, i);
            }
        }
    }
    return result;
}

void kl::Float3x3::operator*=(const Float3x3& other)
{
    *this = (*this * other);
}

kl::Float3 kl::Float3x3::operator*(const Float3& vec) const
{
    Float3 result = {};
    for (int y = 0; y < 3; y++) {
        for (int i = 0; i < 3; i++) {
            result[y] += (*this)(i, y) * vec[i];
        }
    }
    return result;
}

float kl::Float3x3::determinant() const
{
    const float result =
        (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(1, 2)) -
        (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) +
        (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
    return 1.0f / result;
}

// Static
kl::Float3x3 kl::Float3x3::scaling(const Float2& scale)
{
    Float3x3 result = {};
    result[0] = scale.x;
    result[4] = scale.y;
    return result;
}

kl::Float3x3 kl::Float3x3::rotation(const float rotation)
{
    const float z_sin = std::sin(rotation * TO_RADIANS);
    const float z_cos = std::cos(rotation * TO_RADIANS);

    Float3x3 result = {};
    result[0] = z_cos;
    result[1] = -z_sin;
    result[3] = z_sin;
    result[4] = z_cos;
    return result;
}

kl::Float3x3 kl::Float3x3::translation(const Float2& translation)
{
    Float3x3 result = {};
    result[2] = translation.x;
    result[5] = translation.y;
    return result;
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const Float3x3& mat)
{
    stream << format_matrix<3, 3>(mat.data);
    return stream;
}
