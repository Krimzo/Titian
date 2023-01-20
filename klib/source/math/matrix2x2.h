#pragma once

#include "math/matrix.h"


namespace kl {
    template <typename T>
    struct matrix2x2 : matrix<T, 2, 2>
    {
        matrix2x2()
        {}

        matrix2x2(const vector2<T>& r0, const vector2<T>& r1)
        {
            (*this)[0] = r0.x;
            (*this)[1] = r0.y;
            (*this)[2] = r1.x;
            (*this)[3] = r1.y;
        }

        matrix2x2(const matrix<T, 2, 2>& obj)
            : matrix<T, 2, 2>(obj)
        {}
    };
}

namespace kl {
    using float2x2 = matrix2x2<float>;
    using double2x2 = matrix2x2<double>;

    using mat2 = float2x2;
    using dmat2 = double2x2;
}
