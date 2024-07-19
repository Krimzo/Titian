#pragma once

#include "components/animation.h"


namespace titian {
    class Scene;
}

namespace titian {
    class DefaultAnimations
    {
    public:
        kl::Object<Animation> cube = nullptr;
        kl::Object<Animation> sphere = nullptr;
        kl::Object<Animation> capsule = nullptr;
        kl::Object<Animation> monke = nullptr;

        DefaultAnimations(kl::GPU* gpu, Scene* scene);
    };
}
