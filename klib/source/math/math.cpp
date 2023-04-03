#include "math/math.h"


// Deg trig
float kl::sin_deg(const float angle_in_deg)
{
    return sin(angle_in_deg * to_radians);
}

float kl::cos_deg(const float angle_in_deg)
{
    return cos(angle_in_deg * to_radians);
}

float kl::tan_deg(const float angle_in_deg)
{
    return tan(angle_in_deg * to_radians);
}

float kl::asin_deg(const float value)
{
    return asin(value) * to_degrees;
}

float kl::acos_deg(const float value)
{
    return acos(value) * to_degrees;
}

float kl::atan_deg(const float value)
{
    return atan(value) * to_degrees;
}

// Lines
float kl::line_x(const float2& a, const float2& b, const float y)
{
    return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
}

float kl::line_y(const float2& a, const float2& b, const float x)
{
    return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
}

// Normalization
float kl::normalize(float value, const float lower, const float upper)
{
    value = (value - lower) / (upper - lower);
    value = min(max(value, 0.0f), 1.0f);
    return value;
}

float kl::interpolate(float value, const float lower, const float upper)
{
    value = (upper - lower) * value + lower;
    value = min(max(value, lower), upper);
    return value;
}

// Rotation
kl::quaternion kl::to_quaternion(const float3& euler)
{
    const float cr = cos_deg(euler.x * 0.5f);
    const float sr = sin_deg(euler.x * 0.5f);
    const float cp = cos_deg(euler.y * 0.5f);
    const float sp = sin_deg(euler.y * 0.5f);
    const float cy = cos_deg(euler.z * 0.5f);
    const float sy = sin_deg(euler.z * 0.5f);

    return {
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
    };
}

kl::float3 kl::to_euler(const quaternion& quaternion)
{
    const float       sin_p = +2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x) + 0.0f;
    const float sin_r_cos_p = +2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z) + 0.0f;
    const float cos_r_cos_p = -2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y) + 1.0f;
    const float sin_y_cos_p = +2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y) + 0.0f;
    const float cos_y_cos_p = -2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z) + 1.0f;

    const float3 result = {
        atan2(sin_r_cos_p, cos_r_cos_p),
        (::abs(sin_p) >= 1.0f) ? copysign(pi * 0.5f, sin_p) : asin(sin_p),
        atan2(sin_y_cos_p, cos_y_cos_p),
    };
    return (result * to_degrees);
}

// Complex
kl::complex kl::abs(const complex& num)
{
    return { ::abs(num.r), ::abs(num.i) };
}

kl::complex kl::normalize(const complex& num)
{
    return num * (1.0f / num.length());
}

kl::complex kl::inverse(const complex& num)
{
    const float sqr_sum = (num.r * num.r + num.i * num.i);
    if (sqr_sum == 0.0f) {
        return {};
    }
    return { num.r / sqr_sum, -num.i / sqr_sum };
}

// Quaternion
kl::quaternion kl::abs(const quaternion& num)
{
    return { ::abs(num.w), ::abs(num.x), ::abs(num.y), ::abs(num.z) };
}

kl::quaternion kl::normalize(const quaternion& num)
{
    return num * (1.0f / num.length());
}

kl::quaternion kl::inverse(const quaternion& num)
{
    return { num.w, -num.x, -num.y, -num.z };
}

// Int2
kl::int2 kl::abs(const int2& vec)
{
    return { ::abs(vec.x), ::abs(vec.y) };
}

// Float2
kl::float2 kl::abs(const float2& vec)
{
    return { ::abs(vec.x), ::abs(vec.y) };
}

kl::float2 kl::normalize(const float2& vec)
{
    return (vec * (1.0f / vec.length()));
}

float kl::dot(const float2& first, const float2& second)
{
    return (first.x * second.x + first.y * second.y);
}

float kl::angle(const float2& first, const float2& second, const bool full)
{
    float result = 0.0f;
    if (full) {
        result = atan2(first.x * second.y - first.y * second.x, first.x * second.x + first.y * second.y);
    }
    else {
        result = acos(dot(normalize(first), normalize(second)));
    }
    return result * to_degrees;
}

kl::float2 kl::rotate(const float2& vec, const float angle)
{
    const float sin_a = sin(angle * to_radians);
    const float cos_a = cos(angle * to_radians);
    return { cos_a * vec.x - sin_a * vec.y, sin_a * vec.x + cos_a * vec.y };
}

kl::float2 kl::reflect(const float2& vec, const float2& point)
{
    const float2 normalized = normalize(point);
    return vec - (normalized * (vec * normalized) * 2.0f);
}

// Float3
kl::float3 kl::abs(const float3& vec)
{
    return { ::abs(vec.x), ::abs(vec.y), ::abs(vec.z) };
}

kl::float3 kl::normalize(const float3& vec)
{
    return (vec * (1.0f / vec.length()));
}

float kl::dot(const float3& first, const float3& second)
{
    return (first.x * second.x + first.y * second.y + first.z * second.z);
}

float kl::angle(const float3& first, const float3& second)
{
    return acos(dot(normalize(first), normalize(second))) * to_degrees;
}

kl::float3 kl::rotate(const float3& vec, const float3& axis, const float angle)
{
    const quaternion quat = quaternion(axis, angle);
    const quaternion inv_quat = inverse(quat);
    return (quat * vec * inv_quat);
}

kl::float3 kl::reflect(const float3& first, float3 normal)
{
    normal = normalize(normal);
    return first - (normal * (first * normal) * 2.0f);
}

kl::float3 kl::cross(const float3& first, const float3& second)
{
    return {
        first.y * second.z - first.z * second.y,
        first.z * second.x - first.x * second.z,
        first.x * second.y - first.y * second.x,
    };
}

// Float4
kl::float4 kl::abs(const float4& vec)
{
    return { ::abs(vec.x), ::abs(vec.y), ::abs(vec.z), ::abs(vec.w) };
}

kl::float4 kl::normalize(const float4& vec)
{
    return (vec * (1.0f / vec.length()));
}

float kl::dot(const float4& first, const float4& second)
{
    return (first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w);
}

float kl::angle(const float4& first, const float4& second)
{
    return acos(dot(normalize(first), normalize(second))) * to_degrees;
}

// Float2x2
kl::float2x2 kl::abs(const float2x2& mat)
{
    float2x2 result = {};
    for (int i = 0; i < 4; i++) {
        result[i] = ::abs(mat[i]);
    }
    return result;
}

kl::float2x2 kl::inverse(const float2x2& mat)
{
    const float determinant = mat.determinant();

    float2x2 result = {};
    result[0] = +mat[3];
    result[1] = -mat[1];
    result[2] = -mat[2];
    result[3] = +mat[0];
    return result * determinant;
}

kl::float2x2 kl::transpose(const float2x2& mat)
{
    kl::float2x2 result = {};
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            result(x, y) = mat(y, x);
        }
    }
    return result;
}

// Float3x3
kl::float3x3 kl::abs(const float3x3& mat)
{
    float3x3 result = {};
    for (int i = 0; i < 9; i++) {
        result[i] = ::abs(mat[i]);
    }
    return result;
}

kl::float3x3 kl::inverse(const float3x3& mat)
{
    const float determinant = mat.determinant();

    float3x3 result = {};
    result(0, 0) = (mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2)) * determinant;
    result(0, 1) = (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)) * determinant;
    result(0, 2) = (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) * determinant;
    result(1, 0) = (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)) * determinant;
    result(1, 1) = (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)) * determinant;
    result(1, 2) = (mat(1, 0) * mat(0, 2) - mat(0, 0) * mat(1, 2)) * determinant;
    result(2, 0) = (mat(1, 0) * mat(2, 1) - mat(2, 0) * mat(1, 1)) * determinant;
    result(2, 1) = (mat(2, 0) * mat(0, 1) - mat(0, 0) * mat(2, 1)) * determinant;
    result(2, 2) = (mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1)) * determinant;
    return result;
}

kl::float3x3 kl::transpose(const float3x3& mat)
{
    kl::float3x3 result = {};
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            result(x, y) = mat(y, x);
        }
    }
    return result;
}

// Float4x4
kl::float4x4 kl::abs(const float4x4& mat)
{
    float4x4 result = {};
    for (int i = 0; i < 16; i++) {
        result[i] = ::abs(mat[i]);
    }
    return result;
}

kl::float4x4 kl::inverse(const float4x4& mat)
{
    const float determinant = mat.determinant();

    const float A2323 = mat(2, 2) * mat(3, 3) - mat(3, 2) * mat(2, 3);
    const float A1323 = mat(1, 2) * mat(3, 3) - mat(3, 2) * mat(1, 3);
    const float A1223 = mat(1, 2) * mat(2, 3) - mat(2, 2) * mat(1, 3);
    const float A0323 = mat(0, 2) * mat(3, 3) - mat(3, 2) * mat(0, 3);
    const float A0223 = mat(0, 2) * mat(2, 3) - mat(2, 2) * mat(0, 3);
    const float A0123 = mat(0, 2) * mat(1, 3) - mat(1, 2) * mat(0, 3);
    const float A2313 = mat(2, 1) * mat(3, 3) - mat(3, 1) * mat(2, 3);
    const float A1313 = mat(1, 1) * mat(3, 3) - mat(3, 1) * mat(1, 3);
    const float A1213 = mat(1, 1) * mat(2, 3) - mat(2, 1) * mat(1, 3);
    const float A2312 = mat(2, 1) * mat(3, 2) - mat(3, 1) * mat(2, 2);
    const float A1312 = mat(1, 1) * mat(3, 2) - mat(3, 1) * mat(1, 2);
    const float A1212 = mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2);
    const float A0313 = mat(0, 1) * mat(3, 3) - mat(3, 1) * mat(0, 3);
    const float A0213 = mat(0, 1) * mat(2, 3) - mat(2, 1) * mat(0, 3);
    const float A0312 = mat(0, 1) * mat(3, 2) - mat(3, 1) * mat(0, 2);
    const float A0212 = mat(0, 1) * mat(2, 2) - mat(2, 1) * mat(0, 2);
    const float A0113 = mat(0, 1) * mat(1, 3) - mat(1, 1) * mat(0, 3);
    const float A0112 = mat(0, 1) * mat(1, 2) - mat(1, 1) * mat(0, 2);

    float4x4 result = {};
    result(0, 0) = +(mat(1, 1) * A2323 - mat(2, 1) * A1323 + mat(3, 1) * A1223) * determinant;
    result(1, 0) = -(mat(1, 0) * A2323 - mat(2, 0) * A1323 + mat(3, 0) * A1223) * determinant;
    result(2, 0) = +(mat(1, 0) * A2313 - mat(2, 0) * A1313 + mat(3, 0) * A1213) * determinant;
    result(3, 0) = -(mat(1, 0) * A2312 - mat(2, 0) * A1312 + mat(3, 0) * A1212) * determinant;
    result(0, 1) = -(mat(0, 1) * A2323 - mat(2, 1) * A0323 + mat(3, 1) * A0223) * determinant;
    result(1, 1) = +(mat(0, 0) * A2323 - mat(2, 0) * A0323 + mat(3, 0) * A0223) * determinant;
    result(2, 1) = -(mat(0, 0) * A2313 - mat(2, 0) * A0313 + mat(3, 0) * A0213) * determinant;
    result(3, 1) = +(mat(0, 0) * A2312 - mat(2, 0) * A0312 + mat(3, 0) * A0212) * determinant;
    result(0, 2) = +(mat(0, 1) * A1323 - mat(1, 1) * A0323 + mat(3, 1) * A0123) * determinant;
    result(1, 2) = -(mat(0, 0) * A1323 - mat(1, 0) * A0323 + mat(3, 0) * A0123) * determinant;
    result(2, 2) = +(mat(0, 0) * A1313 - mat(1, 0) * A0313 + mat(3, 0) * A0113) * determinant;
    result(3, 2) = -(mat(0, 0) * A1312 - mat(1, 0) * A0312 + mat(3, 0) * A0112) * determinant;
    result(0, 3) = -(mat(0, 1) * A1223 - mat(1, 1) * A0223 + mat(2, 1) * A0123) * determinant;
    result(1, 3) = +(mat(0, 0) * A1223 - mat(1, 0) * A0223 + mat(2, 0) * A0123) * determinant;
    result(2, 3) = -(mat(0, 0) * A1213 - mat(1, 0) * A0213 + mat(2, 0) * A0113) * determinant;
    result(3, 3) = +(mat(0, 0) * A1212 - mat(1, 0) * A0212 + mat(2, 0) * A0112) * determinant;
    return result;
}

kl::float4x4 kl::transpose(const float4x4& mat)
{
    kl::float4x4 result = {};
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            result(x, y) = mat(y, x);
        }
    }
    return result;
}
