#pragma once

#include "components/mesh.h"
#include "utility/async_util.h"


namespace titian {
    struct Scene;
}

namespace titian {
    struct DefaultMeshes
	{
        Ref<Mesh> cube = nullptr;
        Ref<Mesh> sphere = nullptr;
        Ref<Mesh> capsule = nullptr;
        Ref<Mesh> monke = nullptr;

        DefaultMeshes(Scene* scene);
	};
}
