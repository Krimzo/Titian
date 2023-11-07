#include "main.h"


titian::DefaultMeshes::DefaultMeshes(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking)
{
    cube = new Mesh(gpu, physics, cooking);
    sphere = new Mesh(gpu, physics, cooking);
    capsule = new Mesh(gpu, physics, cooking);
    monke = new Mesh(gpu, physics, cooking);

    cube->data_buffer = kl::parse_obj_file("builtin/meshes/cube.obj");
    sphere->data_buffer = kl::parse_obj_file("builtin/meshes/sphere.obj");
    capsule->data_buffer = kl::parse_obj_file("builtin/meshes/capsule.obj");
    monke->data_buffer = kl::parse_obj_file("builtin/meshes/monke.obj");

    cube->reload();
    sphere->reload();
    capsule->reload();
    monke->reload();
}
