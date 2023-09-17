#pragma once

#include "math/imaginary/complex.h"


namespace kl {
    class Quaternion
    {
    public:
        float w = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        // Construct
        Quaternion();
        Quaternion(float x, float y, float z);
        Quaternion(float w, float x, float y, float z);

        explicit Quaternion(const Float3& axis, float angle);

        // Get
        operator float* () const;
        operator kl::Float3() const;
        operator kl::Float4() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const Quaternion& other) const;
        bool operator!=(const Quaternion& other) const;

        // Math
        Quaternion operator+(const Quaternion& other) const;
        void operator+=(const Quaternion& other);

        Quaternion operator-(const Quaternion& other) const;
        void operator-=(const Quaternion& other);

        Quaternion operator*(float value) const;
        void operator*=(float value);

        Quaternion operator*(const Quaternion& other) const;
        void operator*=(const Quaternion& other);

        // Other
        Quaternion operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Quaternion& num);
}
