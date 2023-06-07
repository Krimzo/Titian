#pragma once

#include "graphics/graphics.h"


namespace kl {
    using mesh_data = std::vector<vertex>;
}

#ifdef KL_USING_PHYSX

namespace kl {
    class scene;
}

namespace kl {
    class mesh
    {
        void free_physics_buffer();

    public:
        gpu* gpu = nullptr;
        scene* scene = nullptr;

        mesh_data data_buffer = {};
        dx::buffer graphics_buffer = nullptr;
        PxTriangleMesh* physics_buffer = nullptr;

        mesh(kl::gpu* gpu, kl::scene* scene);
        mesh(kl::gpu* gpu, kl::scene* scene, const mesh_data& data);
        ~mesh();

        void reload();
    };
}

#else

namespace kl {
    class mesh
    {
    public:
        gpu* gpu = nullptr;
        mesh_data data_buffer = {};
        dx::buffer graphics_buffer = nullptr;

        mesh(kl::gpu* gpu);
        mesh(kl::gpu* gpu, const mesh_data& data);

        void reload();
    };
}

#endif
