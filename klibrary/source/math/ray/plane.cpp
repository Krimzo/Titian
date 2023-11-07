#include "klibrary.h"


// Class
kl::Plane::Plane()
{}

kl::Plane::Plane(const Float3& origin, const Float3& normal)
    : origin(origin)
{
    set_normal(normal);
}

// Normal
void kl::Plane::set_normal(const Float3& normal)
{
    m_normal = normalize(normal);
}

kl::Float3 kl::Plane::normal() const
{
    return m_normal;
}

// Math
bool kl::Plane::in_front(const Float3& point) const
{
    const float result = dot(point - origin, m_normal);
    return (result >= 0.0f);
}

// Format
std::ostream& kl::operator<<(std::ostream& os, const kl::Plane& obj)
{
    os << "{" << obj.origin << ", " << obj.normal() << "}";
    return os;
}
