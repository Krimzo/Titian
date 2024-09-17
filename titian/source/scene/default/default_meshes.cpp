#include "titian.h"


titian::DefaultMeshes::DefaultMeshes(Scene* scene)
{
    kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

    WorkQueue queue;
    queue.add_task([&]
    {
        cube = new Mesh(scene, gpu);
        cube->load_triangles(kl::GPU::generate_cube_mesh(1.0f));
    });
    queue.add_task([&]
    {
        sphere = new Mesh(scene, gpu);
        sphere->load_triangles(kl::GPU::generate_sphere_mesh(1.0f, 3, true));
    });
    queue.add_task([&]
    {
        capsule = new Mesh(scene, gpu);
        capsule->load_triangles(kl::GPU::generate_capsule_mesh(1.0f, 2.0f, 16, 16));
    });
    queue.finalize();
}
