#include "main.h"


titian::DefaultAnimations::DefaultAnimations(kl::GPU* gpu, Scene* scene)
{
    auto create_animation = [&](kl::Object<Animation>& animation, std::initializer_list<std::string> meshes)
    {
        animation = new Animation(gpu, scene);
        animation->meshes.insert(animation->meshes.end(), meshes);
    };

    create_animation(cube, { "cube" });
    create_animation(sphere, { "sphere" });
    create_animation(capsule, { "capsule" });
    create_animation(monke, { "monke" });
}
