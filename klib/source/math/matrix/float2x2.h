#pragma once

#include "math/vector/float4.h"


namespace kl {
    struct float2x2
    {
        float data[4] = {
            1.0f, 0.0f,
            0.0f, 1.0f,
        };

        // Construct
        float2x2();

        // Get
        float& operator[](int index);
        const float& operator[](int index) const;

        float& operator()(int x, int y);
        const float& operator()(int x, int y) const;

        // Compare
        bool operator==(const float2x2& other) const;
        bool operator!=(const float2x2& other) const;

        // Math
        float2x2 operator+(const float2x2& other) const;
        void operator+=(const float2x2& other);

        float2x2 operator-(const float2x2& other) const;
        void operator-=(const float2x2& other);

        float2x2 operator*(float value) const;
        void operator*=(float value);

        float2x2 operator*(const float2x2& other) const;
        void operator*=(const float2x2& other);

        float2 operator*(const float2& vec) const;

        float determinant() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const float2x2& mat);
}
