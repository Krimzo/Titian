#include "klibrary.h"


// Deg trig
float kl::sin_deg(const float angle_in_deg)
{
    return sin(angle_in_deg * TO_RADIANS);
}

float kl::cos_deg(const float angle_in_deg)
{
    return cos(angle_in_deg * TO_RADIANS);
}

float kl::tan_deg(const float angle_in_deg)
{
    return tan(angle_in_deg * TO_RADIANS);
}

float kl::asin_deg(const float value)
{
    return asin(value) * TO_DEGREES;
}

float kl::acos_deg(const float value)
{
    return acos(value) * TO_DEGREES;
}

float kl::atan_deg(const float value)
{
    return atan(value) * TO_DEGREES;
}

// Coords
kl::Float2 kl::calc_ndc(const Float2& position, const Float2& frame_size)
{
    const Float2 result = {
        position.x / frame_size.x,
        (frame_size.y - position.y) / frame_size.y,
    };
    return result * 2.0f - Float2(1.0f);
}

kl::Float2 kl::calc_ndc_ar(const Float2& position, const Float2& size)
{
    Float2 result = calc_ndc(position, size);
    result.x *= (size.x / size.y);
    return result;
}

// Lines
float kl::line_x(const Float2& a, const Float2& b, const float y)
{
    return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
}

float kl::line_y(const Float2& a, const Float2& b, const float x)
{
    return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
}

// Wrap
float kl::wrap(float value, const float lower, const float upper)
{
    value = (value - lower) / (upper - lower);
    value = std::clamp(value, 0.0f, 1.0f);
    return value;
}

float kl::unwrap(float value, const float lower, const float upper)
{
    value = (upper - lower) * value + lower;
    value = std::clamp(value, lower, upper);
    return value;
}

float kl::clamp(const float value, const float lower, const float upper)
{
    return std::clamp(value, lower, upper);
}

// Rotation
kl::Quaternion kl::to_quaternion(const Float3& euler)
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

kl::Float3 kl::to_euler(const Quaternion& quaternion)
{
    const float       sin_p = +2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x) + 0.0f;
    const float sin_r_cos_p = +2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z) + 0.0f;
    const float cos_r_cos_p = -2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y) + 1.0f;
    const float sin_y_cos_p = +2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y) + 0.0f;
    const float cos_y_cos_p = -2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z) + 1.0f;

    const Float3 result = {
        atan2(sin_r_cos_p, cos_r_cos_p),
        (::abs(sin_p) >= 1.0f) ? copysign(PI * 0.5f, sin_p) : asin(sin_p),
        atan2(sin_y_cos_p, cos_y_cos_p),
    };
    return (result * TO_DEGREES);
}

// Complex
kl::Complex kl::abs(const Complex& num)
{
    return { ::abs(num.r), ::abs(num.i) };
}

kl::Complex kl::normalize(const Complex& num)
{
    return num * (1.0f / num.length());
}

kl::Complex kl::inverse(const Complex& num)
{
    const float sqr_sum = (num.r * num.r + num.i * num.i);
    if (sqr_sum == 0.0f) {
        return {};
    }
    return { num.r / sqr_sum, -num.i / sqr_sum };
}

// Quaternion
kl::Quaternion kl::abs(const Quaternion& num)
{
    return { ::abs(num.w), ::abs(num.x), ::abs(num.y), ::abs(num.z) };
}

kl::Quaternion kl::normalize(const Quaternion& num)
{
    return num * (1.0f / num.length());
}

kl::Quaternion kl::inverse(const Quaternion& num)
{
    return { num.w, -num.x, -num.y, -num.z };
}

// Int2
kl::Int2 kl::abs(const Int2& vec)
{
    return { ::abs(vec.x), ::abs(vec.y) };
}

// Float2
kl::Float2 kl::abs(const Float2& vec)
{
    return { ::abs(vec.x), ::abs(vec.y) };
}

kl::Float2 kl::normalize(const Float2& vec)
{
    return (vec * (1.0f / vec.length()));
}

float kl::dot(const Float2& first, const Float2& second)
{
    return (first.x * second.x + first.y * second.y);
}

float kl::angle(Float2 first, Float2 second, const bool full)
{
    first = normalize(first);
    second = normalize(second);

    float result {};
    if (full) {
        result = atan2(first.x * second.y - first.y * second.x, first.x * second.x + first.y * second.y);
        result += PI;
    }
    else {
        result = acos(dot(first, second));
    }
    return result * TO_DEGREES;
}

kl::Float2 kl::rotate(const Float2& vec, const float angle)
{
    const float sin_a = sin(angle * TO_RADIANS);
    const float cos_a = cos(angle * TO_RADIANS);
    return { cos_a * vec.x - sin_a * vec.y, sin_a * vec.x + cos_a * vec.y };
}

kl::Float2 kl::reflect(const Float2& vec, const Float2& point)
{
    const Float2 normalized = normalize(point);
    return vec - (normalized * (vec * normalized) * 2.0f);
}

// Float3
kl::Float3 kl::abs(const Float3& vec)
{
    return { ::abs(vec.x), ::abs(vec.y), ::abs(vec.z) };
}

kl::Float3 kl::normalize(const Float3& vec)
{
    return (vec * (1.0f / vec.length()));
}

float kl::dot(const Float3& first, const Float3& second)
{
    return (first.x * second.x + first.y * second.y + first.z * second.z);
}

float kl::angle(const Float3& first, const Float3& second)
{
    return acos(dot(normalize(first), normalize(second))) * TO_DEGREES;
}

kl::Float3 kl::rotate(const Float3& vec, const Float3& axis, const float angle)
{
    const Quaternion quat = Quaternion(axis, angle);
    const Quaternion inv_quat = inverse(quat);
    return (quat * vec * inv_quat);
}

kl::Float3 kl::reflect(const Float3& first, Float3 normal)
{
    normal = normalize(normal);
    return first - (normal * (first * normal) * 2.0f);
}

kl::Float3 kl::cross(const Float3& first, const Float3& second)
{
    return {
        first.y * second.z - first.z * second.y,
        first.z * second.x - first.x * second.z,
        first.x * second.y - first.y * second.x,
    };
}

// Float4
kl::Float4 kl::abs(const Float4& vec)
{
    return { ::abs(vec.x), ::abs(vec.y), ::abs(vec.z), ::abs(vec.w) };
}

kl::Float4 kl::normalize(const Float4& vec)
{
    return (vec * (1.0f / vec.length()));
}

float kl::dot(const Float4& first, const Float4& second)
{
    return (first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w);
}

float kl::angle(const Float4& first, const Float4& second)
{
    return acos(dot(normalize(first), normalize(second))) * TO_DEGREES;
}

// Float2x2
kl::Float2x2 kl::abs(const Float2x2& mat)
{
    Float2x2 result = {};
    for (int i = 0; i < 4; i++) {
        result[i] = ::abs(mat[i]);
    }
    return result;
}

kl::Float2x2 kl::inverse(const Float2x2& mat)
{
    const float determinant = mat.determinant();

    Float2x2 result = {};
    result[0] = +mat[3];
    result[1] = -mat[1];
    result[2] = -mat[2];
    result[3] = +mat[0];
    return result * determinant;
}

kl::Float2x2 kl::transpose(const Float2x2& mat)
{
    kl::Float2x2 result = {};
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            result(x, y) = mat(y, x);
        }
    }
    return result;
}

// Float3x3
kl::Float3x3 kl::abs(const Float3x3& mat)
{
    Float3x3 result = {};
    for (int i = 0; i < 9; i++) {
        result[i] = ::abs(mat[i]);
    }
    return result;
}

kl::Float3x3 kl::inverse(const Float3x3& mat)
{
    const float determinant = mat.determinant();

    Float3x3 result = {};
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

kl::Float3x3 kl::transpose(const Float3x3& mat)
{
    kl::Float3x3 result = {};
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            result(x, y) = mat(y, x);
        }
    }
    return result;
}

// Float4x4
kl::Float4x4 kl::abs(const Float4x4& mat)
{
    Float4x4 result = {};
    for (int i = 0; i < 16; i++) {
        result[i] = ::abs(mat[i]);
    }
    return result;
}

kl::Float4x4 kl::inverse(const Float4x4& mat)
{
    const float determinant = mat.determinant();

    const float a_2323 = mat(2, 2) * mat(3, 3) - mat(3, 2) * mat(2, 3);
    const float a_1323 = mat(1, 2) * mat(3, 3) - mat(3, 2) * mat(1, 3);
    const float a_1223 = mat(1, 2) * mat(2, 3) - mat(2, 2) * mat(1, 3);
    const float a_0323 = mat(0, 2) * mat(3, 3) - mat(3, 2) * mat(0, 3);
    const float a_0223 = mat(0, 2) * mat(2, 3) - mat(2, 2) * mat(0, 3);
    const float a_0123 = mat(0, 2) * mat(1, 3) - mat(1, 2) * mat(0, 3);
    const float a_2313 = mat(2, 1) * mat(3, 3) - mat(3, 1) * mat(2, 3);
    const float a_1313 = mat(1, 1) * mat(3, 3) - mat(3, 1) * mat(1, 3);
    const float a_1213 = mat(1, 1) * mat(2, 3) - mat(2, 1) * mat(1, 3);
    const float a_2312 = mat(2, 1) * mat(3, 2) - mat(3, 1) * mat(2, 2);
    const float a_1312 = mat(1, 1) * mat(3, 2) - mat(3, 1) * mat(1, 2);
    const float a_1212 = mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2);
    const float a_0313 = mat(0, 1) * mat(3, 3) - mat(3, 1) * mat(0, 3);
    const float a_0213 = mat(0, 1) * mat(2, 3) - mat(2, 1) * mat(0, 3);
    const float a_0312 = mat(0, 1) * mat(3, 2) - mat(3, 1) * mat(0, 2);
    const float a_0212 = mat(0, 1) * mat(2, 2) - mat(2, 1) * mat(0, 2);
    const float a_0113 = mat(0, 1) * mat(1, 3) - mat(1, 1) * mat(0, 3);
    const float a_0112 = mat(0, 1) * mat(1, 2) - mat(1, 1) * mat(0, 2);

    Float4x4 result{};
    result(0, 0) = +(mat(1, 1) * a_2323 - mat(2, 1) * a_1323 + mat(3, 1) * a_1223) * determinant;
    result(1, 0) = -(mat(1, 0) * a_2323 - mat(2, 0) * a_1323 + mat(3, 0) * a_1223) * determinant;
    result(2, 0) = +(mat(1, 0) * a_2313 - mat(2, 0) * a_1313 + mat(3, 0) * a_1213) * determinant;
    result(3, 0) = -(mat(1, 0) * a_2312 - mat(2, 0) * a_1312 + mat(3, 0) * a_1212) * determinant;
    result(0, 1) = -(mat(0, 1) * a_2323 - mat(2, 1) * a_0323 + mat(3, 1) * a_0223) * determinant;
    result(1, 1) = +(mat(0, 0) * a_2323 - mat(2, 0) * a_0323 + mat(3, 0) * a_0223) * determinant;
    result(2, 1) = -(mat(0, 0) * a_2313 - mat(2, 0) * a_0313 + mat(3, 0) * a_0213) * determinant;
    result(3, 1) = +(mat(0, 0) * a_2312 - mat(2, 0) * a_0312 + mat(3, 0) * a_0212) * determinant;
    result(0, 2) = +(mat(0, 1) * a_1323 - mat(1, 1) * a_0323 + mat(3, 1) * a_0123) * determinant;
    result(1, 2) = -(mat(0, 0) * a_1323 - mat(1, 0) * a_0323 + mat(3, 0) * a_0123) * determinant;
    result(2, 2) = +(mat(0, 0) * a_1313 - mat(1, 0) * a_0313 + mat(3, 0) * a_0113) * determinant;
    result(3, 2) = -(mat(0, 0) * a_1312 - mat(1, 0) * a_0312 + mat(3, 0) * a_0112) * determinant;
    result(0, 3) = -(mat(0, 1) * a_1223 - mat(1, 1) * a_0223 + mat(2, 1) * a_0123) * determinant;
    result(1, 3) = +(mat(0, 0) * a_1223 - mat(1, 0) * a_0223 + mat(2, 0) * a_0123) * determinant;
    result(2, 3) = -(mat(0, 0) * a_1213 - mat(1, 0) * a_0213 + mat(2, 0) * a_0113) * determinant;
    result(3, 3) = +(mat(0, 0) * a_1212 - mat(1, 0) * a_0212 + mat(2, 0) * a_0112) * determinant;
    return result;
}

kl::Float4x4 kl::transpose(const Float4x4& mat)
{
    kl::Float4x4 result{};
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            result(x, y) = mat(y, x);
        }
    }
    return result;
}
