#include "klibrary.h"


// Class
kl::Sphere::Sphere()
{}

kl::Sphere::Sphere(const Float3& origin, const float radius)
    : origin(origin), radius(radius)
{}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::Sphere& obj)
{
    os << "{" << obj.origin << ", " << obj.radius << "}";
    return os;
}
