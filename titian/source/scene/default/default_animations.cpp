#include "titian.h"


titian::DefaultAnimations::DefaultAnimations()
{
    const auto create_animation = [&](Ref<Animation>& animation, std::initializer_list<String> meshes)
    {
        animation = new Animation();
        animation->meshes.insert(animation->meshes.end(), meshes);
    };

    create_animation(cube, { "cube" });
    create_animation(sphere, { "sphere" });
    create_animation(capsule, { "capsule" });
}
