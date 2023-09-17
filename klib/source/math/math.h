#pragma once

// Helper
#include "math/helper/constants.h"

// Imaginary
#include "math/imaginary/complex.h"
#include "math/imaginary/quaternion.h"

// Vector
#include "math/vector/int2.h"
#include "math/vector/float2.h"
#include "math/vector/float3.h"
#include "math/vector/float4.h"

// Matrix
#include "math/matrix/float2x2.h"
#include "math/matrix/float3x3.h"
#include "math/matrix/float4x4.h"

// Triangle
#include "math/triangle/vertex.h"
#include "math/triangle/triangle.h"

// Ray
#include "math/ray/plane.h"
#include "math/ray/sphere.h"
#include "math/ray/ray.h"


namespace kl {
    // Deg trig
    float sin_deg(float angle_in_deg);
    float cos_deg(float angle_in_deg);
    float tan_deg(float angle_in_deg);

    float asin_deg(float value);
    float acos_deg(float value);
    float atan_deg(float value);

    // Coords
    Float2 calc_ndc(const Float2& position, const Float2& size);
    Float2 calc_ndc_ar(const Float2& position, const Float2& size);

    // Lines
    float line_x(const Float2& a, const Float2& b, float y);
    float line_y(const Float2& a, const Float2& b, float x);

    // Wrap
    float wrap(float value, float lower, float upper);
    float unwrap(float value, float lower, float upper);
    float clamp(float value, float lower, float upper);

    // Rotation
    Quaternion to_quaternion(const Float3& euler);
    Float3 to_euler(const Quaternion& quaternion);

    // Complex
    Complex abs(const Complex& num);
    Complex normalize(const Complex& num);
    Complex inverse(const Complex& num);

    // Quaternion
    Quaternion abs(const Quaternion& num);
    Quaternion normalize(const Quaternion& num);
    Quaternion inverse(const Quaternion& num);

    // Int2 operations
    Int2 abs(const Int2& vec);

    // Float2 operations
    Float2 abs(const Float2& vec);
    Float2 normalize(const Float2& vec);
    float dot(const Float2& first, const Float2& second);
    float angle(Float2 first, Float2 second, bool full);
    Float2 rotate(const Float2& vec, float angle);
    Float2 reflect(const Float2& vec, const Float2& point);

    // Float3 operations
    Float3 abs(const Float3& vec);
    Float3 normalize(const Float3& vec);
    float dot(const Float3& first, const Float3& second);
    float angle(const Float3& first, const Float3& second);
    Float3 rotate(const Float3& vec, const Float3& axis, float angle);
    Float3 reflect(const Float3& first, Float3 normal);
    Float3 cross(const Float3& first, const Float3& second);

    // Float4 operations
    Float4 abs(const Float4& vec);
    Float4 normalize(const Float4& vec);
    float dot(const Float4& first, const Float4& second);
    float angle(const Float4& first, const Float4& second);

    // Float2x2
    Float2x2 abs(const Float2x2& mat);
    Float2x2 inverse(const Float2x2& mat);
    Float2x2 transpose(const Float2x2& mat);

    // Float3x3
    Float3x3 abs(const Float3x3& mat);
    Float3x3 inverse(const Float3x3& mat);
    Float3x3 transpose(const Float3x3& mat);

    // Float4x4
    Float4x4 abs(const Float4x4& mat);
    Float4x4 inverse(const Float4x4& mat);
    Float4x4 transpose(const Float4x4& mat);
}
