#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    struct Sphere
    {
        Vector3<T> position;
        T radius = {};

        constexpr Sphere()
        {}

        constexpr Sphere(const Vector3<T>& position, T radius)
			: position(position) , radius(radius)
        {}
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Sphere<T>& sphere)
    {
        stream << "{" << sphere.position << ", " << sphere.radius << "}";
        return stream;
    }
}
