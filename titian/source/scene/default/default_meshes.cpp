#include "titian.h"


titian::DefaultMeshes::DefaultMeshes()
{
    WorkQueue queue;
    queue.add_task([&]
    {
        cube = new Mesh();
        cube->load_triangles(kl::GPU::generate_cube_mesh(1.0f));
    });
    queue.add_task([&]
    {
        sphere = new Mesh();
        sphere->load_triangles(kl::GPU::generate_sphere_mesh(1.0f, 3, true));
    });
    queue.add_task([&]
    {
        capsule = new Mesh();
        capsule->load_triangles(kl::GPU::generate_capsule_mesh(1.0f, 2.0f, 16, 16));
    });
    queue.finalize();
}
