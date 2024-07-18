#pragma once

#include "components/mesh.h"
#include "utility/async_util.h"


namespace titian {
    class Scene;
}

namespace titian {
	class DefaultMeshes
	{
	public:
        kl::Object<Mesh> cube = nullptr;
        kl::Object<Mesh> sphere = nullptr;
        kl::Object<Mesh> capsule = nullptr;
        kl::Object<Mesh> monke = nullptr;

        DefaultMeshes(Scene* scene);
	};
}
