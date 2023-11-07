#pragma once

#include "math/helper/constants.h"
#include "media/image/color.h"


namespace kl {
    class Float2;
}

namespace kl {
    class Int2
    {
    public:
        int x = 0;
        int y = 0;

        // Construct
        Int2();
        explicit Int2(int value);
        Int2(int x, int y);

        // Get
        operator int* ();
        operator const int* () const;
        
        operator kl::Float2() const;

        int& operator[](int index);
        const int& operator[](int index) const;

        // Compare
        bool operator==(const Int2& other) const;
        bool operator!=(const Int2& other) const;

        // Math
        Int2 operator+(const Int2& other) const;
        void operator+=(const Int2& other);

        Int2 operator-(const Int2& other) const;
        void operator-=(const Int2& other);

        Int2 operator*(int value) const;
        void operator*=(int value);

        Int2 operator*(const Int2& other) const;
        void operator*=(const Int2& other);

        Int2 operator/(int value) const;
        void operator/=(int value);

        Int2 operator/(const Int2& other) const;
        void operator/=(const Int2& other);

        // Other
        Int2 operator-() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Int2& vec);
}
