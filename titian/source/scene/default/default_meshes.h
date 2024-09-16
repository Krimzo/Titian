#pragma once

#include "components/mesh.h"
#include "utility/async_util.h"


namespace titian {
    struct Scene;
}

namespace titian {
    struct DefaultMeshes
	{
        Ref<Mesh> cube;
        Ref<Mesh> sphere;
        Ref<Mesh> capsule;

        DefaultMeshes(Scene* scene);
	};
}
