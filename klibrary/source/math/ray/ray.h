#pragma once

#include "math/ray/plane.h"
#include "math/ray/sphere.h"
#include "math/triangle/triangle.h"
#include "math/matrix/matrix4x4.h"


namespace kl {
    template<typename T = float>
    class Ray
    {
        Vector3<T> m_direction = { T(0), T(0), T(1) };

    public:
        Vector3<T> origin;

        // Construct
        constexpr Ray()
        {}

        constexpr Ray(const Vector3<T>& origin, const Vector3<T>& direction)
			: origin(origin)
        {
            set_direction(direction);
        }

        constexpr Ray(const Vector3<T>& origin, const Matrix4x4<T>& inverse_matrix, const Vector2<T>& ndc)
            : origin(origin)
        {
            Vector4<T> pixel_direction = inverse_matrix * Vector4<T>(ndc, T(1), T(1));
            pixel_direction *= T(1) / pixel_direction.w;
            set_direction(pixel_direction.xyz());
        }

        // Direction
        constexpr void set_direction(const Vector3<T>& direction)
        {
			m_direction = normalize(direction);
        }

        constexpr Vector3<T> direction() const
        {
			return m_direction;
        }

        // Intersection
        constexpr bool intersect_plane(const Plane<T>& plane, Vector3<T>* out_intersection) const
        {
            const T denom = dot(plane.normal(), m_direction);
            if (abs(denom) <= T(0.0001)) {
                return false;
            }

            const T t = dot(plane.origin - origin, plane.normal()) / denom;
            if (t < T(0)) {
                return false;
            }

            if (out_intersection) {
                *out_intersection = origin + m_direction * t;
            }
            return true;
        }

        constexpr bool intersect_triangle(const Triangle<T>& triangle, Vector3<T>* out_intersection) const
        {
            const Vector3<T> edge1 = triangle.b.world - triangle.a.world;
            const Vector3<T> edge2 = triangle.c.world - triangle.a.world;

            const Vector3<T> h = cross(m_direction, edge2);
            const Vector3<T> s = origin - triangle.a.world;
            const T f = T(1) / dot(edge1, h);
            const T u = dot(s, h) * f;
            if (u < T(0) || u > T(1)) {
                return false;
            }

            const Vector3<T> q = cross(s, edge1);
            const T v = dot(m_direction, q) * f;
            if (v < T(0) || (u + v) > T(1)) {
                return false;
            }

            const T t = dot(edge2, q) * f;
            if (t <= T(0)) {
                return false;
            }

            if (out_intersection) {
                *out_intersection = origin + m_direction * t;
            }
            return true;
        }

        constexpr bool intersect_aabb(const Vector3<T>& aabb_min, const Vector3<T>& aabb_max, Vector3<T>* out_intersection) const
        {
            const Vector3<T> inv_ray{ T(1) / m_direction.x, T(1) / m_direction.y, T(1) / m_direction.z };
            const Vector3<T> t1 = (aabb_min - origin) * inv_ray;
            const Vector3<T> t2 = (aabb_max - origin) * inv_ray;

            const Vector3<T> t_min{ min(t1.x, t2.x), min(t1.y, t2.y), min(t1.z, t2.z) };
            const Vector3<T> t_max{ max(t1.x, t2.x), max(t1.y, t2.y), max(t1.z, t2.z) };
            const T t_min_max = max({ t_min.x, t_min.y, t_min.z });
            const T t_max_min = min({ t_max.x, t_max.y, t_max.z });

            if (t_max_min < T(0) || t_min_max > t_max_min) {
                return false;
            }
            if (out_intersection) {
                *out_intersection = origin + m_direction * t_min_max;
            }
            return true;
        }

        constexpr bool can_intersect_sphere(const Sphere<T>& sphere) const
        {
            const T ray_distance = dot((sphere.origin - origin), m_direction);
            const Vector3<T> ray_point = origin + m_direction * ray_distance;
            const T sphere_ray_distance = (sphere.origin - ray_point).length();
            return sphere_ray_distance <= sphere.radius;
        }

        constexpr bool intersect_sphere(const Sphere<T>& sphere, Vector3<T>* out_intersection, T* out_distance) const
        {
            const Vector3<T> center_ray = sphere.origin - origin;
            const T cd_dot = dot(center_ray, m_direction);
            if (cd_dot < T(0)) {
                return false;
            }

            const T cc_dot = dot(center_ray, center_ray) - cd_dot * cd_dot;
            const T rr = sphere.radius * sphere.radius;
            if (cc_dot > rr) {
                return false;
            }

            const T thc = std::sqrt(rr - cc_dot);
            const T dis0 = cd_dot - thc;
            const T dis1 = cd_dot + thc;

            if (out_intersection) {
                *out_intersection = origin + m_direction * (dis0 < T(0) ? dis1 : dis0);
            }
            if (out_distance) {
                *out_distance = dis0 < T(0) ? dis1 : dis0;
            }
            return true;
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Ray<T>& ray)
    {
        stream << "{" << ray.origin << ", " << ray.direction() << "}";
        return stream;
    }
}
