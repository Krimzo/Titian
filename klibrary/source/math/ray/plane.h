#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    struct Plane
    {
        Vector3<T> position;

        constexpr Plane()
        {}

        constexpr Plane(const Vector3<T>& position, const Vector3<T>& normal)
			: position(position)
        {
			set_normal(normal);
        }

        constexpr void set_normal(const Vector3<T>& normal)
		{
			m_normal = normalize(normal);
		}

        constexpr Vector3<T> normal() const
		{
			return m_normal;
		}

        constexpr bool in_front(const Vector3<T>& point) const
        {
            const T result = dot(point - position, m_normal);
            return result >= T(0);
        }

    private:
        Vector3<T> m_normal = { T(0), T(1), T(0) };
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Plane<T>& plane)
    {
        stream << "{" << plane.position << ", " << plane.normal() << "}";
        return stream;
    }
}
