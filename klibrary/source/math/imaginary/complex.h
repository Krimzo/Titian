#pragma once

#include "math/vector/float4.h"


namespace kl {
    class Complex
    {
    public:
        float r = 0.0f;
        float i = 0.0f;

        // Construct
        Complex();
        Complex(float r, float i);

        explicit Complex(float angle);

        // Get
        operator float* () const;
        operator kl::Float2() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const Complex& other) const;
        bool operator!=(const Complex& other) const;

        // Math
        Complex operator+(const Complex& other) const;
        void operator+=(const Complex& other);

        Complex operator-(const Complex& other) const;
        void operator-=(const Complex& other);

        Complex operator*(float value) const;
        void operator*=(float value);

        Complex operator*(const Complex& other) const;
        void operator*=(const Complex& other);

        // Other
        Complex operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Complex& num);
}
