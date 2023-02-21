#include "math/ray/sphere.h"


// Class
kl::sphere::sphere()
{}

kl::sphere::sphere(const float3& origin, float radius)
    : origin(origin), radius(radius)
{}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::sphere& obj)
{
    os << "{" << obj.origin << ", " << obj.radius << "}";
    return os;
}
