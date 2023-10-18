#pragma once

#include "math/vector/float3.h"


namespace kl {
    class Float4
    {
    public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        // Construct
        Float4();
        explicit Float4(float value);
        Float4(float x, float y, float z, float w);

        Float4(const Float2& vec, float z, float w);
        Float4(float x, const Float2& vec, float w);
        Float4(float x, float y, const Float2& vec);
        Float4(const Float2& vec1, const Float2& vec2);

        Float4(const Float3& vec, float w);
        Float4(float x, const Float3& vec);

        // Get
        operator float* ();
        operator const float* () const;

        operator kl::Color() const;
        operator kl::Quaternion() const;

        Float2 xy() const;
        Float3 xyz() const;

        float& operator[](int index);
        const float& operator[](int index) const;

        // Compare
        bool operator==(const Float4& other) const;
        bool operator!=(const Float4& other) const;

        // Math
        Float4 operator+(const Float4& other) const;
        void operator+=(const Float4& other);

        Float4 operator-(const Float4& other) const;
        void operator-=(const Float4& other);

        Float4 operator*(float value) const;
        void operator*=(float value);

        Float4 operator*(const Float4& other) const;
        void operator*=(const Float4& other);

        Float4 operator/(float value) const;
        void operator/=(float value);

        Float4 operator/(const Float4& other) const;
        void operator/=(const Float4& other);

        // Other
        Float4 operator-() const;
        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Float4& vec);
}
