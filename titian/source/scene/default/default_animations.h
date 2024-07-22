#pragma once

#include "components/animation.h"


namespace titian {
    class Scene;
}

namespace titian {
    class DefaultAnimations
    {
    public:
        Ref<Animation> cube = nullptr;
        Ref<Animation> sphere = nullptr;
        Ref<Animation> capsule = nullptr;
        Ref<Animation> monke = nullptr;

        DefaultAnimations(kl::GPU* gpu, Scene* scene);
    };
}
