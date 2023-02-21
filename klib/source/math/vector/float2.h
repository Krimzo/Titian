#pragma once

#include "math/vector/int2.h"


namespace kl {
    struct float2
    {
        float x = 0.0f;
        float y = 0.0f;

        // Construct
        float2();
        explicit float2(float value);
        float2(float x, float y);

        // Get
        operator float* () const;
        operator kl::int2() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const float2& other) const;
        bool operator!=(const float2& other) const;

        // Math
        float2 operator+(const float2& other) const;
        void operator+=(const float2& other);

        float2 operator-(const float2& other) const;
        void operator-=(const float2& other);

        float2 operator*(float value) const;
        void operator*=(float value);

        float operator*(const float2& other) const;

        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const float2& vec);
}
