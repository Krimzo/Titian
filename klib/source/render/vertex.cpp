#include "render/vertex.h"


kl::vertex::vertex()
{}

kl::vertex::vertex(const float3& world)
    : world(world)
{}

kl::vertex::vertex(const float3& world, const float2& texture)
    : world(world), texture(texture)
{}

kl::vertex::vertex(const float3& world, const float3& normal)
    : world(world), normal(normal)
{}

kl::vertex::vertex(const float3& world, const float2& texture, const float3& normal)
    : world(world), texture(texture), normal(normal)
{}

std::ostream& kl::operator<<(std::ostream& stream, const vertex& obj)
{
    stream << "{" << obj.world << ", " << obj.texture << ", " << obj.normal << "}";
    return stream;
}
