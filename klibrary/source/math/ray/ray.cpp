#include "klibrary.h"


// Construct
kl::Ray::Ray()
{}

kl::Ray::Ray(const Float3& origin, const Float3& direction)
    : origin(origin)
{
    set_direction(direction);
}

kl::Ray::Ray(const Float3& origin, const Float4x4& inverse_matrix, const Float2& ndc)
    : origin(origin)
{
    Float4 pixel_direction = inverse_matrix * Float4(ndc, 1.0f, 1.0f);
    pixel_direction *= 1.0f / pixel_direction.w;
    set_direction(pixel_direction.xyz());
}

// Direction
void kl::Ray::set_direction(const Float3& direction)
{
    m_direction = normalize(direction);
}

kl::Float3 kl::Ray::direction() const
{
    return m_direction;
}

// Intersection
bool kl::Ray::intersect_plane(const Plane& plane, Float3* out_intersection) const
{
    const float denom = dot(plane.normal(), m_direction);
    if (::abs(denom) <= 0.0001f) {
        return false;
    }

    const float t = dot(plane.origin - origin, plane.normal()) / denom;
    if (t < 0.0f) {
        return false;
    }

    if (out_intersection) {
        *out_intersection = origin + m_direction * t;
    }
    return true;
}

bool kl::Ray::intersect_triangle(const Triangle& triangle, Float3* out_intersection) const
{
    const Float3 edge1 = triangle.b.world - triangle.a.world;
    const Float3 edge2 = triangle.c.world - triangle.a.world;

    const Float3 h = cross(m_direction, edge2);
    const Float3 s = origin - triangle.a.world;
    const float f = 1.0f / dot(edge1, h);
    const float u = dot(s, h) * f;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    const Float3 q = cross(s, edge1);
    const float v = dot(m_direction, q) * f;
    if (v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    const float t = dot(edge2, q) * f;
    if (t <= 0.0f) {
        return false;
    }

    if (out_intersection) {
        *out_intersection = origin + m_direction * t;
    }
    return true;
}

bool kl::Ray::intersect_box(const Float3& box_min, const Float3& box_max, Float3* out_intersection) const
{
    const Float3 inv_ray{ 1.0f / m_direction.x, 1.0f / m_direction.y, 1.0f / m_direction.z };

    const Float3 t1 = (box_min - origin) * inv_ray;
    const Float3 t2 = (box_max - origin) * inv_ray;

    const Float3 t_min{ std::min(t1.x, t2.x), std::min(t1.y, t2.y), std::min(t1.z, t2.z) };
    const Float3 t_max{ std::max(t1.x, t2.x), std::max(t1.y, t2.y), std::max(t1.z, t2.z) };

    const float t_min_max = std::max(std::max(t_min.x, t_min.y), t_min.z);
    const float t_max_min = std::min(std::min(t_max.x, t_max.y), t_max.z);

    if (t_min_max > t_max_min) {
        return false;
    }
    if (out_intersection) {
        *out_intersection = origin + m_direction * t_min_max;
    }
    return true;
}

bool kl::Ray::can_intersect_sphere(const Sphere& sphere) const
{
    const float ray_distance = dot((sphere.origin - origin), m_direction);
    const Float3 ray_point = origin + m_direction * ray_distance;
    const float sphere_ray_distance = (sphere.origin - ray_point).length();
    return !(sphere_ray_distance > sphere.radius);
}

bool kl::Ray::intersect_sphere(const Sphere& sphere, Float3* out_intersection, float* out_distance) const
{
    const Float3 center_ray = sphere.origin - origin;
    const float cd_dot = dot(center_ray, m_direction);
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
        *out_intersection = origin + m_direction * ((dis0 < 0.0f) ? dis1 : dis0);
    }
    if (out_distance) {
        *out_distance = (dis0 < 0.0f) ? dis1 : dis0;
    }
    return true;
}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::Ray& obj)
{
    os << "{" << obj.origin << ", " << obj.direction() << "}";
    return os;
}
