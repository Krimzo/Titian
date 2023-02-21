#pragma once

#include "math/vector/float3.h"


namespace kl {
    struct float4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        // Construct
        float4();
        explicit float4(float value);
        float4(float x, float y, float z, float w);

        float4(const float2& vec, float z, float w);
        float4(float x, const float2& vec, float w);
        float4(float x, float y, const float2& vec);
        float4(const float2& vec1, const float2& vec2);

        float4(const float3& vec, float w);
        float4(float x, const float3& vec);

        // Get
        operator float* () const;
        operator kl::color() const;

        float2 xy() const;
        float3 xyz() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const float4& other) const;
        bool operator!=(const float4& other) const;

        // Math
        float4 operator+(const float4& other) const;
        void operator+=(const float4& other);

        float4 operator-(const float4& other) const;
        void operator-=(const float4& other);

        float4 operator*(float value) const;
        void operator*=(float value);

        float operator*(const float4& other) const;

        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const float4& vec);
}
