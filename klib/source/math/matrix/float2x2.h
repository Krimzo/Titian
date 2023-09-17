#pragma once

#include "math/vector/float4.h"


namespace kl {
    class Float2x2
    {
    public:
        float data[4] = {
            1.0f, 0.0f,
            0.0f, 1.0f,
        };

        // Construct
        Float2x2();

        // Get
        float& operator[](int index);
        const float& operator[](int index) const;

        float& operator()(int x, int y);
        const float& operator()(int x, int y) const;

        // Compare
        bool operator==(const Float2x2& other) const;
        bool operator!=(const Float2x2& other) const;

        // Math
        Float2x2 operator+(const Float2x2& other) const;
        void operator+=(const Float2x2& other);

        Float2x2 operator-(const Float2x2& other) const;
        void operator-=(const Float2x2& other);

        Float2x2 operator*(float value) const;
        void operator*=(float value);

        Float2x2 operator*(const Float2x2& other) const;
        void operator*=(const Float2x2& other);

        Float2 operator*(const Float2& vec) const;

        float determinant() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Float2x2& mat);
}
