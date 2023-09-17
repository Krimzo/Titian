#include "klib.h"


#ifdef KL_USING_PHYSX

kl::Mesh::Mesh(GPU* gpu, Scene* scene)
    : gpu(gpu), scene(scene)
{}

kl::Mesh::Mesh(GPU* gpu, Scene* scene, const MeshData& data)
    : gpu(gpu), scene(scene), data_buffer(data)
{
    reload();
}

kl::Mesh::~Mesh()
{
    free_physics_buffer();
}

void kl::Mesh::reload()
{
    // Graphics
    graphics_buffer = gpu->create_vertex_buffer(data_buffer);

    // Physics
    free_physics_buffer();

    physx::PxTriangleMeshDesc mesh_descriptor = {};
    mesh_descriptor.points.count = (physx::PxU32) data_buffer.size();
    mesh_descriptor.points.stride = (physx::PxU32) sizeof(Vertex);
    mesh_descriptor.points.data = data_buffer.data();

    physx::PxDefaultMemoryOutputStream cook_buffer = {};
    scene->cooking()->cookTriangleMesh(mesh_descriptor, cook_buffer);

    physx::PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
    physics_buffer = scene->physics()->createTriangleMesh(cooked_buffer);
    error_check(!physics_buffer, "Failed to create mesh physics buffer");
}

void kl::Mesh::free_physics_buffer()
{
    if (physics_buffer) {
        physics_buffer->release();
        physics_buffer = nullptr;
    }
}

#else

kl::Mesh::Mesh(kl::GPU* gpu)
    : gpu(gpu)
{}

kl::Mesh::Mesh(kl::GPU* gpu, const MeshData& data)
    : gpu(gpu), data_buffer(data)
{
    reload();
}

void kl::Mesh::reload()
{
    graphics_buffer = gpu->create_vertex_buffer(data_buffer);
}

#endif
