#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T>
    struct Sphere_T
    {
        union { Vector3<T> position; struct { T x, y, z; }; };
        T radius = {};

        constexpr Sphere_T()
        {}

        constexpr Sphere_T(const Vector3<T>& position, T radius)
			: position(position) , radius(radius)
        {}

        constexpr bool contains(const Vector3<T>& point) const
        {
			return (point - position).length() <= radius;
        }
    };
}

namespace kl {
    using Sphere = Sphere_T<float>;
    using SphereD = Sphere_T<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Sphere_T<T>& sphere)
    {
        stream << "{" << sphere.position << ", " << sphere.radius << "}";
        return stream;
    }
}
