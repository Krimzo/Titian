#pragma once

#include "math/matrix/float2x2.h"


namespace kl {
    class Float3x3
    {
    public:
        float data[9] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };

        // Construct
        Float3x3();

        // Get
        float& operator[](int index);
        const float& operator[](int index) const;

        float& operator()(int x, int y);
        const float& operator()(int x, int y) const;

        // Compare
        bool operator==(const Float3x3& other) const;
        bool operator!=(const Float3x3& other) const;

        // Math
        Float3x3 operator+(const Float3x3& other) const;
        void operator+=(const Float3x3& other);

        Float3x3 operator-(const Float3x3& other) const;
        void operator-=(const Float3x3& other);

        Float3x3 operator*(float value) const;
        void operator*=(float value);

        Float3x3 operator*(const Float3x3& other) const;
        void operator*=(const Float3x3& other);

        Float3 operator*(const Float3& vec) const;

        float determinant() const;

        // Static
        static Float3x3 scaling(const Float2& scale);
        static Float3x3 rotation(float rotation);
        static Float3x3 translation(const Float2& translation);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Float3x3& mat);
}
