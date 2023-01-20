#include "render/ray.h"


kl::ray::ray()
{}

kl::ray::ray(const float3& origin, const float3& direction)
    : origin(origin), direction(direction.normalize())
{}

kl::ray::ray(const float3& origin, const mat4& inverse_matrix, const float2& ndc)
    : origin(origin)
{
    const float4 pixel_direction = inverse_matrix * float4(ndc, 1.0f, 1.0f);
    direction = (pixel_direction / pixel_direction.w).xyz.normalize();
}

kl::ray::ray(const camera& camera, const float2& ndc)
    : ray(camera.position, camera.matrix().inverse(), ndc)
{}

bool kl::ray::intersect_plane(const plane& plane, float3* out_intersection) const
{
    const float denom = plane.normal.normalize().dot(direction.normalize());
    if (std::abs(denom) > 0.0001f) {
        if (const float t = (plane.point - origin).dot(plane.normal) / denom; t >= 0.0f) {
            if (out_intersection) {
                *out_intersection = origin + direction * t;
            }
            return true;
        }
    }
    return false;
}

bool kl::ray::intersect_triangle(const triangle& triangle, float3* out_intersection) const
{
    const float3 edge1 = triangle.b.world - triangle.a.world;
    const float3 edge2 = triangle.c.world - triangle.a.world;

    const float3 h = direction.cross(edge2);
    const float3 s = origin - triangle.a.world;
    const float f = 1.0f / edge1.dot(h);
    const float u = s.dot(h) * f;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    const float3 q = s.cross(edge1);
    if (const float v = direction.dot(q) * f; v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    if (const float t = edge2.dot(q) * f; t > 0.0f) {
        if (out_intersection) {
            *out_intersection = origin + direction * t;
        }
        return true;
    }
    return false;
}

bool kl::ray::intersect_sphere(const sphere& sphere, float3& out_intersection, float* out_distance) const
{
    const float3 center_ray = sphere.center - origin;
    const float cd_dot = center_ray.dot(direction);
    if (cd_dot < 0.0f) {
        return false;
    }

    const float cc_dot = center_ray.dot(center_ray) - cd_dot * cd_dot;
    const float rr = sphere.radius * sphere.radius;
    if (cc_dot > rr) {
        return false;
    }

    const float thc = sqrt(rr - cc_dot);
    const float dis0 = cd_dot - thc;
    const float dis1 = cd_dot + thc;

    out_intersection = origin + direction * ((dis0 < 0.0f) ? dis1 : dis0);

    if (out_distance) {
        *out_distance = (dis0 < 0.0f) ? dis1 : dis0;
    }

    return true;
}

bool kl::ray::intersect_sphere(const sphere& sphere) const
{
    const float ray_distance = (sphere.center - origin).dot(direction);
    const float3 ray_point = origin + direction * ray_distance;
    const float sphere_ray_distance = (sphere.center - ray_point).length();
    return !(sphere_ray_distance > sphere.radius);
}

std::ostream& kl::operator<<(std::ostream& os, const ray& obj)
{
    os << "{" << obj.origin << ", " << obj.direction << "}";
    return os;
}
