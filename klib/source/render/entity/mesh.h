#pragma once

#include "math/math.h"
#include "memory/ref.h"
#include "graphics/gpu.h"

#include <vector>

#ifdef KL_USING_PHYSX
#include "PxPhysicsAPI.h"
using namespace physx;
#endif


namespace kl {
    class scene;
}

namespace kl {
#ifdef KL_USING_PHYSX

    class mesh
    {
        gpu* gpu_ = nullptr;

    public:
        std::vector<vertex> vertices = {};
        dx::buffer graphics_buffer = nullptr;
        PxTriangleMesh* physics_buffer = nullptr;

        mesh(gpu* gpu, dx::buffer graphics_buffer);
        mesh(gpu* gpu, const std::vector<vertex>& vertices);
        mesh(gpu* gpu, scene* scene, const std::vector<vertex>& vertices);
        ~mesh();

        mesh(const mesh&) = delete;
        mesh(const mesh&&) = delete;

        void operator=(const mesh&) = delete;
        void operator=(const mesh&&) = delete;
    };

#else

    using mesh_data = std::vector<vertex>;
    using mesh = dx::buffer;

#endif
}
