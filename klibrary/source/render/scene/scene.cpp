#include "klibrary.h"


void kl::Scene::update_physics(const float delta_t)
{
    for (auto& entity : entities) {
        entity->velocity += gravity * delta_t;
        entity->velocity += entity->acceleration * delta_t;
        entity->position += entity->velocity * delta_t;
        entity->rotation += entity->angular * delta_t;
    }
}
