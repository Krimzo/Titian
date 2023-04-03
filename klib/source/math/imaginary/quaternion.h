#pragma once

#include "math/imaginary/complex.h"


namespace kl {
    struct quaternion
    {
        float w = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        // Construct
        quaternion();
        quaternion(float x, float y, float z);
        quaternion(float w, float x, float y, float z);

        explicit quaternion(const float3& axis, float angle);

        // Get
        operator float* () const;
        operator kl::float3() const;
        operator kl::float4() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const quaternion& other) const;
        bool operator!=(const quaternion& other) const;

        // Math
        quaternion operator+(const quaternion& other) const;
        void operator+=(const quaternion& other);

        quaternion operator-(const quaternion& other) const;
        void operator-=(const quaternion& other);

        quaternion operator*(float value) const;
        void operator*=(float value);

        quaternion operator*(const quaternion& other) const;
        void operator*=(const quaternion& other);

        // Other
        quaternion operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const quaternion& num);
}
