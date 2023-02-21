#pragma once

#include "math/matrix/float2x2.h"


namespace kl {
    struct float3x3
    {
        float data[9] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };

        // Construct
        float3x3();

        // Get
        float& operator[](int index);
        const float& operator[](int index) const;

        float& operator()(int x, int y);
        const float& operator()(int x, int y) const;

        // Compare
        bool operator==(const float3x3& other) const;
        bool operator!=(const float3x3& other) const;

        // Math
        float3x3 operator+(const float3x3& other) const;
        void operator+=(const float3x3& other);

        float3x3 operator-(const float3x3& other) const;
        void operator-=(const float3x3& other);

        float3x3 operator*(float value) const;
        void operator*=(float value);

        float3x3 operator*(const float3x3& other) const;
        void operator*=(const float3x3& other);

        float3 operator*(const float3& vec) const;

        float determinant() const;

        // Static
        static float3x3 translation(const float2& val);
        static float3x3 rotation(float rotation);
        static float3x3 scaling(const float2& vec);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const float3x3& mat);
}
