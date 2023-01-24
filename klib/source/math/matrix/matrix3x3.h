#pragma once

#include "math/matrix/matrix.h"


namespace kl {
    template <typename T>
    struct matrix3x3 : matrix<T, 3, 3>
    {
        matrix3x3()
        {}

        matrix3x3(const vector3<T>& r0, const vector3<T>& r1, const vector3<T>& r2)
        {
            (*this)[0] = r0.x;
            (*this)[1] = r0.y;
            (*this)[2] = r0.z;
            (*this)[3] = r1.x;
            (*this)[4] = r1.y;
            (*this)[5] = r1.z;
            (*this)[6] = r2.x;
            (*this)[7] = r2.y;
            (*this)[8] = r2.z;
        }

        matrix3x3(const matrix<T, 3, 3>& obj)
            : matrix<T, 3, 3>(obj)
        {}

        static matrix3x3<T> translation(const vector2<T>& val)
        {
            matrix3x3<T> result;
            result[2] = val.x;
            result[5] = val.y;
            return result;
        }

        static matrix3x3<T> rotation(const T& val)
        {
            const T z_sin = std::sin(math::to_radians(val));
            const T z_cos = std::cos(math::to_radians(val));

            matrix3x3<T> result;
            result[0] = z_cos;
            result[1] = -z_sin;
            result[3] = z_sin;
            result[4] = z_cos;
            return result;
        }

        static matrix3x3<T> scaling(const vector2<T>& val)
        {
            matrix3x3<T> result;
            result[0] = val.x;
            result[4] = val.y;
            return result;
        }
    };
}

namespace kl {
    using float3x3 = matrix3x3<float>;
    using double3x3 = matrix3x3<double>;

    using mat3 = float3x3;
    using dmat3 = double3x3;
}
