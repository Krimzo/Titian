#include "math/matrix/float3x3.h"

#include "utility/utility.h"


// Construct
kl::float3x3::float3x3()
{}

// Get
float& kl::float3x3::operator[](int index)
{
    return data[index];
}

const float& kl::float3x3::operator[](int index) const
{
    return data[index];
}

float& kl::float3x3::operator()(int x, int y)
{
    return data[y * 3 + x];
}

const float& kl::float3x3::operator()(int x, int y) const
{
    return data[y * 3 + x];
}

// Compare
bool kl::float3x3::operator==(const float3x3& other) const
{
    for (int i = 0; i < 9; i++) {
        if (data[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool kl::float3x3::operator!=(const float3x3& other) const
{
    return !(*this == other);
}

// Math
kl::float3x3 kl::float3x3::operator+(const float3x3& other) const
{
    float3x3 result = *this;
    for (int i = 0; i < 9; i++) {
        result[i] += other[i];
    }
    return result;
}

void kl::float3x3::operator+=(const float3x3& other)
{
    for (int i = 0; i < 9; i++) {
        data[i] += other[i];
    }
}

kl::float3x3 kl::float3x3::operator-(const float3x3& other) const
{
    float3x3 result = *this;
    for (int i = 0; i < 9; i++) {
        result[i] -= other[i];
    }
    return result;
}

void kl::float3x3::operator-=(const float3x3& other)
{
    for (int i = 0; i < 9; i++) {
        data[i] -= other[i];
    }
}

kl::float3x3 kl::float3x3::operator*(const float value) const
{
    float3x3 result = *this;
    for (int i = 0; i < 9; i++) {
        result[i] *= value;
    }
    return result;
}

void kl::float3x3::operator*=(const float value)
{
    for (int i = 0; i < 9; i++) {
        data[i] *= value;
    }
}

kl::float3x3 kl::float3x3::operator*(const float3x3& other) const
{
    float3x3 result = {};
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

void kl::float3x3::operator*=(const float3x3& other)
{
    *this = (*this * other);
}

kl::float3 kl::float3x3::operator*(const float3& vec) const
{
    float3 result = {};
    for (int y = 0; y < 3; y++) {
        for (int i = 0; i < 3; i++) {
            const float vec_val = ((const float*) &vec)[i];
            ((float*) &result)[y] += (*this)(i, y) * vec_val;
        }
    }
    return result;
}

float kl::float3x3::determinant() const
{
    const float result =
        (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(1, 2)) -
        (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) +
        (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
    return 1.0f / result;
}

// Static
kl::float3x3 kl::float3x3::translation(const float2& val)
{
    float3x3 result = {};
    result[2] = val.x;
    result[5] = val.y;
    return result;
}

kl::float3x3 kl::float3x3::rotation(float rotation)
{
    const float z_sin = sin(rotation * math::to_radians);
    const float z_cos = cos(rotation * math::to_radians);

    float3x3 result = {};
    result[0] = z_cos;
    result[1] = -z_sin;
    result[3] = z_sin;
    result[4] = z_cos;
    return result;
}

kl::float3x3 kl::float3x3::scaling(const float2& vec)
{
    float3x3 result = {};
    result[0] = vec.x;
    result[4] = vec.y;
    return result;
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const float3x3& mat)
{
    stream << format_matrix<3, 3>(mat.data);
    return stream;
}
