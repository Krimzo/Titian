#include "math/triangle/triangle.h"

#include "math/math.h"


// Construct
kl::triangle::triangle()
{}

kl::triangle::triangle(const vertex& a, const vertex& b, const vertex& c)
    : a(a), b(b), c(c)
{}

// Normal
kl::float3 kl::triangle::get_normal() const
{
    return normalize(cross(b.world - a.world, c.world - a.world));
}

// Interpolation
kl::float4 kl::triangle::get_constants() const
{
    const float calc_const = (b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y);
    const float rec_constant = 1.0f / calc_const;

    return {
        (b.world.y - c.world.y) * rec_constant,
        (c.world.x - b.world.x) * rec_constant,
        (c.world.y - a.world.y) * rec_constant,
        (a.world.x - c.world.x) * rec_constant
    };
}

kl::float3 kl::triangle::get_weights(const float4& constants, const float2& point) const
{
    const float dx = point.x - c.world.x;
    const float dy = point.y - c.world.y;
    const float w1 = dx * constants.x + dy * constants.y;
    const float w2 = dx * constants.z + dy * constants.w;

    return { w1, w2, (1.0f - w1 - w2) };
}

kl::float3 kl::triangle::get_weights(const float3& position) const
{
    const float3 v0 = a.world - c.world;
    const float3 v1 = b.world - c.world;
    const float3 v2 = position - c.world;

    const float d00 = dot(v0, v0);
    const float d01 = dot(v0, v1);
    const float d11 = dot(v1, v1);
    const float d20 = dot(v2, v0);
    const float d21 = dot(v2, v1);

    const float inverse_denom = 1.0f / (d00 * d11 - d01 * d01);
    const float w1 = (d11 * d20 - d01 * d21) * inverse_denom;
    const float w2 = (d00 * d21 - d01 * d20) * inverse_denom;

    return { w1, w2, (1.0f - w1 - w2) };
}

kl::vertex kl::triangle::interpolate_self(const float3& weights) const
{
    return {
        {
            interpolate(weights, { a.world.x, b.world.x, c.world.x }),
            interpolate(weights, { a.world.y, b.world.y, c.world.y }),
            interpolate(weights, { a.world.z, b.world.z, c.world.z })
        },
        {
            interpolate(weights, { a.texture.x, b.texture.x, c.texture.x }),
            interpolate(weights, { a.texture.y, b.texture.y, c.texture.y })
        },
        {
            interpolate(weights, { a.normal.x, b.normal.x, c.normal.x }),
            interpolate(weights, { a.normal.y, b.normal.y, c.normal.y }),
            interpolate(weights, { a.normal.z, b.normal.z, c.normal.z })
        }
    };
}

// Static
bool kl::triangle::is_in_triangle(const float3& weights)
{
    return !(weights.x < 0.0f || weights.y < 0.0f || weights.z < 0.0f);
}

float kl::triangle::interpolate(const float3& weights, const float3& values)
{
    return dot(weights, values);
}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::triangle& obj)
{
    os << "{" << obj.a << ", " << obj.b << ", " << obj.c << "}";
    return os;
}
