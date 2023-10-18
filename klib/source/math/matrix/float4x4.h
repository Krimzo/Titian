#pragma once

#include "math/matrix/float3x3.h"


namespace kl {
    class Float4x4
    {
    public:
        float data[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };

        // Construct
        Float4x4();

        // Get
        float& operator[](int index);
        const float& operator[](int index) const;

        float& operator()(int x, int y);
        const float& operator()(int x, int y) const;

        // Compare
        bool operator==(const Float4x4& other) const;
        bool operator!=(const Float4x4& other) const;

        // Math
        Float4x4 operator+(const Float4x4& other) const;
        void operator+=(const Float4x4& other);

        Float4x4 operator-(const Float4x4& other) const;
        void operator-=(const Float4x4& other);

        Float4x4 operator*(float value) const;
        void operator*=(float value);

        Float4x4 operator*(const Float4x4& other) const;
        void operator*=(const Float4x4& other);

        Float4 operator*(const Float4& vec) const;

        float determinant() const;

        // Static
        static Float4x4 translation(const Float3& translation);
        static Float4x4 rotation(const Float3& rotation);
        static Float4x4 scaling(const Float3& scale);

        static Float4x4 perspective(float field_of_view, float aspect_ratio, float near_plane, float far_plane);
        static Float4x4 orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);

        static Float4x4 look_at(const Float3& position, const Float3& target, const Float3& up);
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Float4x4& mat);
}
