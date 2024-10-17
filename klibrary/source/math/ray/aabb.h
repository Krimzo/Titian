#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T>
    struct AABB_T
    {
        union { Vector3<T> position; struct { T x, y, z; }; };
        Vector3<T> size;

        constexpr AABB_T()
        {}

        constexpr AABB_T(const Vector3<T>& position, const Vector3<T>& size)
            : position(position), size(size)
        {}

        constexpr Vector3<T> min_point() const
        {
			return position - size;
        }

        constexpr Vector3<T> max_point() const
        {
			return position + size;
        }

        constexpr bool contains(const Vector3<T>& point) const
        {
            const Vector3<T> minp = min_point();
			const Vector3<T> maxp = max_point();
            return
                (point.x >= minp.x && point.x <= maxp.x) &&
                (point.y >= minp.y && point.y <= maxp.y) &&
                (point.z >= minp.z && point.z <= maxp.z);
        }
    };
}

namespace kl {
    using AABB = AABB_T<float>;
    using AABBD = AABB_T<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const AABB_T<T>& aabb)
    {
        stream << "{" << aabb.position << ", " << aabb.size << "}";
        return stream;
    }
}
