#include "render/components/mesh.h"

#include "render/render.h"
#include "utility/utility.h"


#ifdef KL_USING_PHYSX

kl::mesh::mesh(kl::gpu* gpu, kl::scene* scene)
    : gpu(gpu), scene(scene)
{}

kl::mesh::mesh(kl::gpu* gpu, kl::scene* scene, const mesh_data& data)
    : gpu(gpu), scene(scene), data_buffer(data)
{
    reload();
}

kl::mesh::~mesh()
{
    free_physics_buffer();
}

void kl::mesh::reload()
{
    // Graphics
    graphics_buffer = gpu->create_vertex_buffer(data_buffer);

    // Physics
    free_physics_buffer();

    PxTriangleMeshDesc mesh_descriptor = {};
    mesh_descriptor.points.count = (PxU32) data_buffer.size();
    mesh_descriptor.points.stride = (PxU32) sizeof(vertex);
    mesh_descriptor.points.data = data_buffer.data();

    PxDefaultMemoryOutputStream cook_buffer = {};
    scene->get_cooking()->cookTriangleMesh(mesh_descriptor, cook_buffer);

    PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
    physics_buffer = scene->get_physics()->createTriangleMesh(cooked_buffer);
    error_check(!physics_buffer, "Failed to create mesh physics buffer");
}

void kl::mesh::free_physics_buffer()
{
    if (physics_buffer) {
        physics_buffer->release();
        physics_buffer = nullptr;
    }
}

#else

kl::mesh::mesh(kl::gpu* gpu)
    : gpu(gpu)
{}

kl::mesh::mesh(kl::gpu* gpu, const mesh_data& data)
    : gpu(gpu), data_buffer(data)
{
    reload();
}

kl::mesh::~mesh()
{}

void kl::mesh::reload()
{
    graphics_buffer = gpu->create_vertex_buffer(data_buffer);
}

#endif
