#include "math/ray/ray.h"

#include "math/math.h"


// Construct
kl::ray::ray()
{}

kl::ray::ray(const float3& origin, const float3& direction)
    : origin(origin), direction(math::normalize(direction))
{}

kl::ray::ray(const float3& origin, const float4x4& inverse_matrix, const float2& ndc)
    : origin(origin)
{
    float4 pixel_direction = inverse_matrix * float4(ndc.x, ndc.y, 1.0f, 1.0f);
    pixel_direction *= 1.0f / pixel_direction.w;
    direction = math::normalize(float3(pixel_direction.x, pixel_direction.y, pixel_direction.z));
}

// Math
bool kl::ray::intersect_plane(const plane& plane, float3* out_intersection) const
{
    const float denom = math::normalize(plane.normal) * math::normalize(direction);
    if (abs(denom) > 0.0001f) {
        const float t = ((plane.origin - origin) * plane.normal) / denom;
        if (t >= 0.0f) {
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

    const float3 h = math::cross(direction, edge2);
    const float3 s = origin - triangle.a.world;
    const float f = 1.0f / (edge1 * h);
    const float u = (s * h) * f;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    const float3 q = math::cross(s, edge1);
    const float v = (direction * q) * f;
    if (v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    const float t = (edge2 * q) * f;
    if (t > 0.0f) {
        if (out_intersection) {
            *out_intersection = origin + direction * t;
        }
        return true;
    }
    return false;
}

bool kl::ray::intersect_sphere(const sphere& sphere, float3& out_intersection, float* out_distance) const
{
    const float3 center_ray = sphere.origin - origin;
    const float cd_dot = center_ray * direction;
    if (cd_dot < 0.0f) {
        return false;
    }

    const float cc_dot = center_ray * center_ray - cd_dot * cd_dot;
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
    const float ray_distance = (sphere.origin - origin) * direction;
    const float3 ray_point = origin + direction * ray_distance;
    const float sphere_ray_distance = (sphere.origin - ray_point).length();
    return !(sphere_ray_distance > sphere.radius);
}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::ray& obj)
{
    os << "{" << obj.origin << ", " << obj.direction << "}";
    return os;
}
