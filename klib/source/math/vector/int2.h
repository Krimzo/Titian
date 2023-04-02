#pragma once

#include "math/helper/constants.h"
#include "media/image/color.h"


namespace kl {
    struct float2;
}

namespace kl {
    struct int2
    {
        int x = 0;
        int y = 0;

        // Construct
        int2();
        explicit int2(int value);
        int2(int x, int y);

        // Get
        operator int* () const;
        operator kl::float2() const;

        int& operator[](int index);
        const int& operator[](int index) const;

        // Compare
        bool operator==(const int2& other) const;
        bool operator!=(const int2& other) const;

        // Math
        int2 operator+(const int2& other) const;
        void operator+=(const int2& other);

        int2 operator-(const int2& other) const;
        void operator-=(const int2& other);

        int2 operator*(int value) const;
        void operator*=(int value);

        int2 operator*(const int2& other) const;
        void operator*=(const int2& other);

        int2 operator/(int value) const;
        void operator/=(int value);

        int2 operator/(const int2& other) const;
        void operator/=(const int2& other);

        // Other
        int2 operator-() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const int2& vec);
}
