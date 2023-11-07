#pragma once

#include "math/vector/float2.h"


namespace kl {
    class Quaternion;
}

namespace kl {
    class Float3
    {
    public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        // Construct
        Float3();
        explicit Float3(float value);
        Float3(float x, float y, float z);

        Float3(const Float2& vec, float z);
        Float3(float x, const Float2& vec);

        // Get
        operator float* ();
        operator const float* () const;
        
        operator kl::Color() const;
        operator kl::Quaternion() const;

        Float2 xy() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const Float3& other) const;
        bool operator!=(const Float3& other) const;

        // Math
        Float3 operator+(const Float3& other) const;
        void operator+=(const Float3& other);

        Float3 operator-(const Float3& other) const;
        void operator-=(const Float3& other);

        Float3 operator*(float value) const;
        void operator*=(float value);

        Float3 operator*(const Float3& other) const;
        void operator*=(const Float3& other);

        Float3 operator/(float value) const;
        void operator/=(float value);

        Float3 operator/(const Float3& other) const;
        void operator/=(const Float3& other);

        // Other
        Float3 operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Float3& vec);
}
