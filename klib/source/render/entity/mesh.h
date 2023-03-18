#pragma once

#include "graphics/graphics.h"
#include "memory/memory.h"

#ifdef KL_USING_PHYSX
#include <PxPhysicsAPI.h>
using namespace physx;
#endif

namespace kl {
    class scene;
}

namespace kl {
    using mesh_data = std::vector<vertex>;

#ifdef KL_USING_PHYSX

    class mesh
    {
        ref<gpu> gpu_ = nullptr;
        ref<scene> scene_ = nullptr;

    public:
        mesh_data buffer = {};
        dx::buffer graphics_buffer = nullptr;
        PxTriangleMesh* physics_buffer = nullptr;

        mesh(ref<gpu> gpu, ref<scene> scene, const mesh_data& vertices);
        virtual ~mesh();
    };

#else

    using mesh = dx::buffer;

#endif
}
