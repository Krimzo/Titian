#include "titian.h"


titian::DefaultAnimations::DefaultAnimations(kl::GPU* gpu, Scene* scene)
{
    const auto create_animation = [&](Ref<Animation>& animation, std::initializer_list<String> meshes)
    {
        animation = new Animation(scene, gpu);
        animation->meshes.insert(animation->meshes.end(), meshes);
    };

    create_animation(cube, { "cube" });
    create_animation(sphere, { "sphere" });
    create_animation(capsule, { "capsule" });
}
