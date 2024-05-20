#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    class Plane
    {
        Vector3<T> m_normal = { T(0), T(1), T(0) };

    public:
        Vector3<T> origin;

        constexpr Plane()
        {}

        constexpr Plane(const Vector3<T>& origin, const Vector3<T>& normal)
			: origin(origin)
        {
			set_normal(normal);
        }

        // Normal
        constexpr void set_normal(const Vector3<T>& normal)
		{
			m_normal = normalize(normal);
		}

        constexpr Vector3<T> normal() const
		{
			return m_normal;
		}

        // Math
        constexpr bool in_front(const Vector3<T>& point) const
        {
            const T result = dot(point - origin, m_normal);
            return result >= T(0);
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Plane<T>& plane)
    {
        stream << "{" << plane.origin << ", " << plane.normal() << "}";
        return stream;
    }
}
