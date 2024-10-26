#pragma once

#include "math/raytracing/plane.h"
#include "math/raytracing/sphere.h"
#include "math/raytracing/aabb.h"
#include "math/triangle/triangle.h"
#include "math/matrix/matrix4x4.h"


namespace kl {
    template<typename T>
    struct Ray_T
    {
        union { Vector3<T> origin; struct { T x, y, z; }; };

        constexpr Ray_T()
        {}

        constexpr Ray_T(const Vector3<T>& origin, const Vector3<T>& direction)
			: origin(origin)
        {
            set_direction(direction);
        }

        constexpr Ray_T(const Vector3<T>& cam_pos, const Matrix4x4<T>& inv_cam, const Vector2<T>& ndc)
            : origin(cam_pos)
        {
            Vector4<T> ndc_pos = inv_cam * Vector4<T>(ndc, T(1), T(1));
            set_direction(ndc_pos.xyz() / ndc_pos.w - cam_pos);
        }

        constexpr void set_direction(const Vector3<T>& direction)
        {
			m_direction = normalize(direction);
        }

        constexpr Vector3<T> direction() const
        {
			return m_direction;
        }

        constexpr bool intersect_plane(const Plane_T<T>& plane, Vector3<T>* out_intersection) const
        {
            const T denom = dot(plane.normal(), m_direction);
            if (abs(denom) <= T(0.0001))
                return false;

            const T t = dot(plane.position - origin, plane.normal()) / denom;
            if (t < T(0))
                return false;

            if (out_intersection) {
                *out_intersection = origin + m_direction * t;
            }
            return true;
        }

        constexpr bool intersect_sphere(const Sphere_T<T>& sphere, Vector3<T>* out_intersection) const
        {
            if (sphere.contains(origin)) {
                if (out_intersection) {
                    *out_intersection = origin;
                }
                return true;
            }

            const Vector3<T> center_ray = sphere.position - origin;
            const T cd_dot = dot(center_ray, m_direction);
            if (cd_dot < T(0))
                return false;

            const T cc_dot = dot(center_ray, center_ray) - cd_dot * cd_dot;
            const T rr = sphere.radius * sphere.radius;
            if (cc_dot > rr)
                return false;

            const T thc = sqrt(rr - cc_dot);
            const T dis0 = cd_dot - thc;
            const T dis1 = cd_dot + thc;
            if (out_intersection) {
                *out_intersection = origin + m_direction * (dis0 < T(0) ? dis1 : dis0);
            }
            return true;
        }

        constexpr bool intersect_aabb(const AABB_T<T>& aabb, Vector3<T>* out_intersection) const
        {
            if (aabb.contains(origin)) {
                if (out_intersection) {
                    *out_intersection = origin;
                }
				return true;
            }

            const Vector3<T> inv_ray{ T(1) / m_direction.x, T(1) / m_direction.y, T(1) / m_direction.z };
            const Vector3<T> t1 = (aabb.min_point() - origin) * inv_ray;
            const Vector3<T> t2 = (aabb.max_point() - origin) * inv_ray;
            const Vector3<T> t_min{ min(t1.x, t2.x), min(t1.y, t2.y), min(t1.z, t2.z) };
            const Vector3<T> t_max{ max(t1.x, t2.x), max(t1.y, t2.y), max(t1.z, t2.z) };
            const T t_min_max = std::max({ t_min.x, t_min.y, t_min.z });
            const T t_max_min = std::min({ t_max.x, t_max.y, t_max.z });
            if (t_max_min < T(0) || t_min_max > t_max_min)
                return false;

            if (out_intersection) {
                *out_intersection = origin + m_direction * t_min_max;
            }
            return true;
        }

        constexpr bool intersect_triangle(const Triangle_T<T>& triangle, Vector3<T>* out_intersection) const
        {
            const Vector3<T> edge1 = triangle.b.position - triangle.a.position;
            const Vector3<T> edge2 = triangle.c.position - triangle.a.position;

            const Vector3<T> h = cross(m_direction, edge2);
            const Vector3<T> s = origin - triangle.a.position;
            const T f = T(1) / dot(edge1, h);
            const T u = dot(s, h) * f;
            if (u < T(0) || u > T(1))
                return false;

            const Vector3<T> q = cross(s, edge1);
            const T v = dot(m_direction, q) * f;
            if (v < T(0) || (u + v) > T(1))
                return false;

            const T t = dot(edge2, q) * f;
            if (t <= T(0))
                return false;

            if (out_intersection) {
                *out_intersection = origin + m_direction * t;
            }
            return true;
        }

    private:
        Vector3<T> m_direction = { T(0), T(0), T(1) };
    };
}

namespace kl {
    using Ray = Ray_T<float>;
	using RayD = Ray_T<double>;
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Ray_T<T>& ray)
    {
        stream << "{" << ray.origin << ", " << ray.direction() << "}";
        return stream;
    }
}
