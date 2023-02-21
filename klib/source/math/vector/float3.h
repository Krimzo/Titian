#pragma once

#include "math/vector/float2.h"


namespace kl {
    struct float3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        // Construct
        float3();
        explicit float3(float value);
        float3(float x, float y, float z);

        float3(const float2& vec, float z);
        float3(float x, const float2& vec);

        // Get
        operator float* () const;
        operator kl::color() const;

        float2 xy() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const float3& other) const;
        bool operator!=(const float3& other) const;

        // Math
        float3 operator+(const float3& other) const;
        void operator+=(const float3& other);

        float3 operator-(const float3& other) const;
        void operator-=(const float3& other);

        float3 operator*(float value) const;
        void operator*=(float value);

        float operator*(const float3& other) const;

        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const float3& vec);
}
