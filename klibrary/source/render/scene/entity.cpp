#include "klibrary.h"


void kl::Entity::update_physics(const float delta_t)
{
    velocity += acceleration * delta_t;
    position += velocity * delta_t;
    rotation += angular * delta_t;
}

kl::Float4x4 kl::Entity::matrix() const
{
    return Float4x4::translation(position) * Float4x4::rotation(rotation) * Float4x4::scaling(scale);
}
