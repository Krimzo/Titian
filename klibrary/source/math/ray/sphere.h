#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    struct Sphere
    {
        Vector3<T> origin;
        T radius = {};

        constexpr Sphere()
        {}

        constexpr Sphere(const Vector3<T>& origin, T radius)
			: origin(origin) , radius(radius)
        {}
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Sphere<T>& sphere)
    {
        stream << "{" << sphere.origin << ", " << sphere.radius << "}";
        return stream;
    }
}
