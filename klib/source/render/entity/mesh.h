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
    public:
        mesh_data vertices = {};
        dx::buffer graphics_buffer = nullptr;
        PxTriangleMesh* physics_buffer = nullptr;

        mesh(dx::buffer graphics_buffer);
        mesh(gpu& gpu, const mesh_data& vertices);
        mesh(gpu& gpu, scene& scene, const mesh_data& vertices);
        virtual ~mesh();
    };

#else

    using mesh = dx::buffer;

#endif
}
