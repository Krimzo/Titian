#include "math/matrix/float4x4.h"

#include "utility/utility.h"


// Construct
kl::float4x4::float4x4()
{}

// Get
float& kl::float4x4::operator[](int index)
{
    return data[index];
}

const float& kl::float4x4::operator[](int index) const
{
    return data[index];
}

float& kl::float4x4::operator()(int x, int y)
{
    return data[y * 4 + x];
}

const float& kl::float4x4::operator()(int x, int y) const
{
    return data[y * 4 + x];
}

// Compare
bool kl::float4x4::operator==(const float4x4& other) const
{
    for (int i = 0; i < 16; i++) {
        if (data[i] != other[i]) {
            return false;
        }
    }
    return true;
}

bool kl::float4x4::operator!=(const float4x4& other) const
{
    return !(*this == other);
}

// Math
kl::float4x4 kl::float4x4::operator+(const float4x4& other) const
{
    float4x4 result = *this;
    for (int i = 0; i < 16; i++) {
        result[i] += other[i];
    }
    return result;
}

void kl::float4x4::operator+=(const float4x4& other)
{
    for (int i = 0; i < 16; i++) {
        data[i] += other[i];
    }
}

kl::float4x4 kl::float4x4::operator-(const float4x4& other) const
{
    float4x4 result = *this;
    for (int i = 0; i < 16; i++) {
        result[i] -= other[i];
    }
    return result;
}

void kl::float4x4::operator-=(const float4x4& other)
{
    for (int i = 0; i < 16; i++) {
        data[i] -= other[i];
    }
}

kl::float4x4 kl::float4x4::operator*(float value) const
{
    float4x4 result = *this;
    for (int i = 0; i < 16; i++) {
        result[i] *= value;
    }
    return result;
}

void kl::float4x4::operator*=(float value)
{
    for (int i = 0; i < 16; i++) {
        data[i] *= value;
    }
}

kl::float4x4 kl::float4x4::operator*(const float4x4& other) const
{
    float4x4 result = {};
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            result(x, y) = 0.0f;
            for (int i = 0; i < 4; i++) {
                result(x, y) += (*this)(i, y) * other(x, i);
            }
        }
    }
    return result;
}

void kl::float4x4::operator*=(const float4x4& other)
{
    *this = (*this * other);
}

kl::float4 kl::float4x4::operator*(const float4& vec) const
{
    float4 result = {};
    for (int y = 0; y < 4; y++) {
        for (int i = 0; i < 4; i++) {
            result[y] += (*this)(i, y) * vec[i];
        }
    }
    return result;
}

float kl::float4x4::determinant() const
{
    const float A2323 = (*this)(2, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(2, 3);
    const float A1323 = (*this)(1, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(1, 3);
    const float A1223 = (*this)(1, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(1, 3);
    const float A0323 = (*this)(0, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(0, 3);
    const float A0223 = (*this)(0, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(0, 3);
    const float A0123 = (*this)(0, 2) * (*this)(1, 3) - (*this)(1, 2) * (*this)(0, 3);

    const float result =
        (*this)(0, 0) * ((*this)(1, 1) * A2323 - (*this)(2, 1) * A1323 + (*this)(3, 1) * A1223) -
        (*this)(1, 0) * ((*this)(0, 1) * A2323 - (*this)(2, 1) * A0323 + (*this)(3, 1) * A0223) +
        (*this)(2, 0) * ((*this)(0, 1) * A1323 - (*this)(1, 1) * A0323 + (*this)(3, 1) * A0123) -
        (*this)(3, 0) * ((*this)(0, 1) * A1223 - (*this)(1, 1) * A0223 + (*this)(2, 1) * A0123);
    return 1.0f / result;
}

// Static
kl::float4x4 kl::float4x4::translation(const float3& translation)
{
    float4x4 result = {};
    result[3] = translation.x;
    result[7] = translation.y;
    result[11] = translation.z;
    return result;
}

kl::float4x4 kl::float4x4::rotation(const float3& rotation)
{
    const float x_rad = rotation.x * to_radians;
    const float x_sin = sin(x_rad);
    const float x_cos = cos(x_rad);

    float4x4 x_rot = {};
    x_rot[5] = x_cos;
    x_rot[6] = -x_sin;
    x_rot[9] = x_sin;
    x_rot[10] = x_cos;

    const float y_rad = rotation.y * to_radians;
    const float y_sin = sin(y_rad);
    const float y_cos = cos(y_rad);

    float4x4 y_rot = {};
    y_rot[0] = y_cos;
    y_rot[2] = y_sin;
    y_rot[8] = -y_sin;
    y_rot[10] = y_cos;

    const float z_rad = rotation.z * to_radians;
    const float z_sin = sin(z_rad);
    const float z_cos = cos(z_rad);

    float4x4 z_rot = {};
    z_rot[0] = z_cos;
    z_rot[1] = -z_sin;
    z_rot[4] = z_sin;
    z_rot[5] = z_cos;

    return z_rot * y_rot * x_rot;
}

kl::float4x4 kl::float4x4::scaling(const float3& scale)
{
    float4x4 result = {};
    result[0] = scale.x;
    result[5] = scale.y;
    result[10] = scale.z;
    return result;
}

kl::float4x4 kl::float4x4::perspective(float field_of_view, float aspect_ratio, float near_plane, float far_plane)
{
    const float tan_half = 1.0f / tan(field_of_view * 0.5f * to_radians);

    float4x4 result = {};
    result[0] = tan_half / aspect_ratio;
    result[5] = tan_half;
    result[10] = (-far_plane - near_plane) / (near_plane - far_plane);
    result[11] = (2.0f * near_plane * far_plane) / (near_plane - far_plane);
    result[14] = 1.0f;
    result[15] = 0.0f;
    return result;
}

kl::float4x4 kl::float4x4::orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane)
{
    float4x4 result = {};
    result[0] = 2.0f / (right - left);
    result[5] = 2.0f / (top - bottom);
    result[10] = 2.0f / (far_plane - near_plane);
    result[3] = -(right + left) / (right - left);
    result[7] = -(top + bottom) / (top - bottom);
    result[11] = -(far_plane + near_plane) / (far_plane - near_plane);
    return result;
}

kl::float4x4 kl::float4x4::look_at(const float3& position, const float3& target, const float3& up)
{
    const float3 f = normalize(target - position);
    const float3 s = normalize(cross(up, f));
    const float3 u = cross(f, s);

    float4x4 result = {};
    result[0] = s.x;
    result[1] = s.y;
    result[2] = s.z;
    result[3] = -dot(s, position);
    result[4] = u.x;
    result[5] = u.y;
    result[6] = u.z;
    result[7] = -dot(u, position);
    result[8] = f.x;
    result[9] = f.y;
    result[10] = f.z;
    result[11] = -dot(f, position);
    return result;
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const float4x4& mat)
{
    stream << format_matrix<4, 4>(mat.data);
    return stream;
}
