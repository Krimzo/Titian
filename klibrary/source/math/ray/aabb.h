#pragma once

#include "math/vector/vector4.h"


namespace kl {
    template<typename T = float>
    struct AABB
    {
        Vector3<T> position;
        Vector3<T> size;

        constexpr AABB()
        {}

        constexpr AABB(const Vector3<T>& position, const Vector3<T>& size)
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
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const AABB<T>& aabb)
    {
        stream << "{" << aabb.position << ", " << aabb.size << "}";
        return stream;
    }
}
