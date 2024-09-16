#pragma once

#include "components/animation.h"


namespace titian {
    struct Scene;
}

namespace titian {
    struct DefaultAnimations
    {
        Ref<Animation> cube;
        Ref<Animation> sphere;
        Ref<Animation> capsule;

        DefaultAnimations(kl::GPU* gpu, Scene* scene);
    };
}
