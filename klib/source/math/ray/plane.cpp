#include "klib.h"


// Class
kl::plane::plane()
{}

kl::plane::plane(const float3& origin, const float3& normal)
    : origin(origin)
{
    set_normal(normal);
}

// Normal
void kl::plane::set_normal(const float3& normal)
{
    normal_ = normalize(normal);
}

kl::float3 kl::plane::normal() const
{
    return normal_;
}

// Math
bool kl::plane::in_front(const float3& point) const
{
    const float result = dot(point - origin, normal_);
    return (result >= 0.0f);
}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::plane& obj)
{
    os << "{" << obj.origin << ", " << obj.normal() << "}";
    return os;
}
