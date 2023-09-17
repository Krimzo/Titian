#pragma once

#include "math/ray/plane.h"
#include "math/ray/sphere.h"
#include "math/triangle/triangle.h"
#include "math/matrix/float4x4.h"


namespace kl {
    class Ray
    {
        Float3 m_direction = { 0.0f, 0.0f, 1.0f };

    public:
        Float3 origin = {};

        // Construct
        Ray();
        Ray(const Float3& origin, const Float3& direction);
        Ray(const Float3& origin, const Float4x4& inverse_matrix, const Float2& ndc);

        // Direction
        void set_direction(const Float3& direction);
        Float3 direction() const;

        // Intersection
        bool intersect_plane(const Plane& plane, Float3* out_intersection) const;
        bool intersect_triangle(const Triangle& triangle, Float3* out_intersection) const;

        bool can_intersect_sphere(const Sphere& sphere) const;
        bool intersect_sphere(const Sphere& sphere, Float3* out_intersection, float* out_distance) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const Ray& obj);
}
