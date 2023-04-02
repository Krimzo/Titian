#include "math/ray/ray.h"

#include "math/math.h"


// Construct
kl::ray::ray()
{}

kl::ray::ray(const float3& origin, const float3& direction)
    : origin(origin)
{
    set_direction(direction);
}

kl::ray::ray(const float3& origin, const float4x4& inverse_matrix, const float2& ndc)
    : origin(origin)
{
    float4 pixel_direction = inverse_matrix * float4(ndc, 1.0f, 1.0f);
    pixel_direction *= 1.0f / pixel_direction.w;
    set_direction(pixel_direction.xyz());
}

// Direction
void kl::ray::set_direction(const float3& direction)
{
    direction_ = normalize(direction);
}

kl::float3 kl::ray::get_direction() const
{
    return direction_;
}

// Intersection
bool kl::ray::intersect_plane(const plane& plane, float3* out_intersection) const
{
    const float denom = dot(plane.get_normal(), direction_);
    if (::abs(denom) <= 0.0001f) {
        return false;
    }

    const float t = dot(plane.origin - origin, plane.get_normal()) / denom;
    if (t < 0.0f) {
        return false;
    }

    if (out_intersection) {
        *out_intersection = origin + direction_ * t;
    }
    return true;
}

bool kl::ray::intersect_triangle(const triangle& triangle, float3* out_intersection) const
{
    const float3 edge1 = triangle.b.world - triangle.a.world;
    const float3 edge2 = triangle.c.world - triangle.a.world;

    const float3 h = cross(direction_, edge2);
    const float3 s = origin - triangle.a.world;
    const float f = 1.0f / dot(edge1, h);
    const float u = dot(s, h) * f;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    const float3 q = cross(s, edge1);
    const float v = dot(direction_, q) * f;
    if (v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    const float t = dot(edge2, q) * f;
    if (t <= 0.0f) {
        return false;
    }

    if (out_intersection) {
        *out_intersection = origin + direction_ * t;
    }
    return true;
}

bool kl::ray::can_intersect_sphere(const sphere& sphere) const
{
    const float ray_distance = dot((sphere.origin - origin), direction_);
    const float3 ray_point = origin + direction_ * ray_distance;
    const float sphere_ray_distance = (sphere.origin - ray_point).length();
    return !(sphere_ray_distance > sphere.radius);
}

bool kl::ray::intersect_sphere(const sphere& sphere, float3* out_intersection, float* out_distance) const
{
    const float3 center_ray = sphere.origin - origin;
    const float cd_dot = dot(center_ray, direction_);
    if (cd_dot < 0.0f) {
        return false;
    }

    const float cc_dot = dot(center_ray, center_ray) - cd_dot * cd_dot;
    const float rr = sphere.radius * sphere.radius;
    if (cc_dot > rr) {
        return false;
    }

    const float thc = sqrt(rr - cc_dot);
    const float dis0 = cd_dot - thc;
    const float dis1 = cd_dot + thc;

    if (out_intersection) {
        *out_intersection = origin + direction_ * ((dis0 < 0.0f) ? dis1 : dis0);
    }
    if (out_distance) {
        *out_distance = (dis0 < 0.0f) ? dis1 : dis0;
    }
    return true;
}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::ray& obj)
{
    os << "{" << obj.origin << ", " << obj.get_direction() << "}";
    return os;
}
