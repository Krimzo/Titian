#include "render/plane.h"


kl::plane::plane()
{}

kl::plane::plane(const float3& normal, const float3& point)
    : normal(normal.normalize()), point(point)
{}

std::ostream& kl::operator<<(std::ostream& os, const plane& obj)
{
    os << "{" << obj.normal << ", " << obj.point << "}";
    return os;
}
