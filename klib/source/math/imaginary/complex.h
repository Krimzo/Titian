#pragma once

#include "math/vector/float4.h"


namespace kl {
    struct complex
    {
        float r = 0.0f;
        float i = 0.0f;

        // Construct
        complex();
        complex(float r, float i);

        explicit complex(float angle);

        // Get
        operator float* () const;
        operator kl::float2() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const complex& other) const;
        bool operator!=(const complex& other) const;

        // Math
        complex operator+(const complex& other) const;
        void operator+=(const complex& other);

        complex operator-(const complex& other) const;
        void operator-=(const complex& other);

        complex operator*(float value) const;
        void operator*=(float value);

        complex operator*(const complex& other) const;
        void operator*=(const complex& other);

        // Other
        complex operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const complex& num);
}
