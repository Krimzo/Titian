#include "render/sphere.h"


kl::sphere::sphere()
{}

kl::sphere::sphere(const float3& center, float radius) : center(center), radius(radius)
{}


std::ostream& kl::operator<<(std::ostream& os, const sphere& obj)
{
    os << "{" << obj.center << ", " << obj.radius << "}";
    return os;
}
