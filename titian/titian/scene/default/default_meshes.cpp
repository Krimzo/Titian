#include "main.h"


titian::DefaultMeshes::DefaultMeshes(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking)
{
    auto create_mesh = [&](kl::Object<Mesh>& mesh, const char* filename)
    {
        mesh = new Mesh(gpu, physics, cooking);
        mesh->load(kl::parse_obj_file(filename));
        mesh->reload();
        kl::assert(mesh->graphics_buffer, "Failed to init mesh: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_mesh(cube, "builtin/meshes/cube.obj"); });
    queue.add_task([&] { create_mesh(sphere, "builtin/meshes/sphere.obj"); });
    queue.add_task([&] { create_mesh(capsule, "builtin/meshes/capsule.obj"); });
    queue.add_task([&] { create_mesh(monke, "builtin/meshes/monke.obj"); });
    queue.finalize();
}
