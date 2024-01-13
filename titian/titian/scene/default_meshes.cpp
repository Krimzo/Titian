#include "main.h"


titian::DefaultMeshes::DefaultMeshes(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking)
{
    const std::initializer_list<std::pair<kl::Object<Mesh>&, const char*>> meshes = {
        { cube, "builtin/meshes/cube.obj" },
        { sphere, "builtin/meshes/sphere.obj" },
        { capsule, "builtin/meshes/capsule.obj" },
        { monke, "builtin/meshes/monke.obj" },
    };
    std::for_each(std::execution::par, meshes.begin(), meshes.end(), [&](auto entry)
    {
        entry.first = new Mesh(gpu, physics, cooking);
        entry.first->data_buffer = kl::parse_obj_file(entry.second);
        entry.first->reload();
    });
}
