#pragma once

#include "graphics/graphics.h"


namespace kl {
    class scene;
    using mesh_data = std::vector<vertex>;
}

namespace kl {
    class mesh
    {
    public:
        mesh_data data_buffer = {};
        dx::buffer graphics_buffer = nullptr;

        mesh();
        mesh(gpu* gpu, const mesh_data& data);

#ifdef KL_USING_PHYSX

        PxTriangleMesh* physics_buffer = nullptr;
        
        mesh(scene* scene, gpu* gpu, const mesh_data& data);
        ~mesh();

#endif

    };
}
