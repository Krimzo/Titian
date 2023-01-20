#include "render/entity.h"


kl::entity::entity()
{}

void kl::entity::update_physics(const float delta_time)
{
    velocity += acceleration * delta_time;
    position += velocity * delta_time;
    rotation += angular * delta_time;
}

kl::mat4 kl::entity::matrix() const
{
    return mat4::translation(position) * mat4::rotation(rotation) * mat4::scaling(scale);
}
