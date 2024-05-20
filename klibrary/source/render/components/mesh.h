#pragma once

#include "graphics/graphics.h"


namespace kl {
    using MeshData = std::vector<Vertex<float>>;
}

#ifdef KL_USING_PHYSX

namespace kl {
    class Scene;
}

namespace kl {
    class Mesh
    {
        void free_physics_buffer();

    public:
        GPU* gpu = nullptr;
        Scene* scene = nullptr;

        MeshData data_buffer = {};
        dx::Buffer graphics_buffer = nullptr;
        physx::PxTriangleMesh* physics_buffer = nullptr;

        Mesh(GPU* gpu, Scene* scene);
        Mesh(GPU* gpu, Scene* scene, const MeshData& data);
        virtual ~Mesh();

        void reload();
    };
}

#else

namespace kl {
    class Mesh
    {
    public:
        GPU* gpu = nullptr;
        MeshData data_buffer = {};
        dx::Buffer graphics_buffer = nullptr;

        Mesh(kl::GPU* gpu);
        Mesh(kl::GPU* gpu, const MeshData& data);

        void reload();
    };
}

#endif
