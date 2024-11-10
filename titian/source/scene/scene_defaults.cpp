#include "titian.h"


titian::DefaultMeshes::DefaultMeshes()
{
    WorkQueue queue;
    queue.add_task( [&]
    {
        cube.load_triangles( kl::GPU::generate_cube_mesh( 1.0f ) );
    } );
    queue.add_task( [&]
    {
        sphere.load_triangles( kl::GPU::generate_sphere_mesh( 1.0f, 3, true ) );
    } );
    queue.add_task( [&]
    {
        capsule.load_triangles( kl::GPU::generate_capsule_mesh( 1.0f, 2.0f, 16, 16 ) );
    } );
    queue.finalize();
}

titian::DefaultAnimations::DefaultAnimations()
{
    auto create_animation = [&]( Animation& animation, std::initializer_list<String> meshes )
    {
        animation.meshes.insert( animation.meshes.end(), meshes );
    };

    create_animation( cube, { "cube" } );
    create_animation( sphere, { "sphere" } );
    create_animation( capsule, { "capsule" } );
}

titian::DefaultMaterials::DefaultMaterials()
{}
