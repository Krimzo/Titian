#pragma once

#include "math/vector/int2.h"


namespace kl {
    class Complex;
}

namespace kl {
    class Float2
    {
    public:
        float x = 0.0f;
        float y = 0.0f;

        // Construct
        Float2();
        explicit Float2(float value);
        Float2(float x, float y);

        // Get
        operator float* ();
        operator const float* () const;
        
        operator kl::Int2() const;
        operator kl::Complex() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const Float2& other) const;
        bool operator!=(const Float2& other) const;

        // Math
        Float2 operator+(const Float2& other) const;
        void operator+=(const Float2& other);

        Float2 operator-(const Float2& other) const;
        void operator-=(const Float2& other);

        Float2 operator*(float value) const;
        void operator*=(float value);

        Float2 operator*(const Float2& other) const;
        void operator*=(const Float2& other);

        Float2 operator/(float value) const;
        void operator/=(float value);

        Float2 operator/(const Float2& other) const;
        void operator/=(const Float2& other);

        // Other
        Float2 operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Float2& vec);
}
