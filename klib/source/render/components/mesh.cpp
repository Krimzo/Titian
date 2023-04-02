#include "render/components/mesh.h"

#include "render/render.h"
#include "utility/utility.h"


kl::mesh::mesh()
{}

kl::mesh::mesh(gpu* gpu, const mesh_data& data)
    : data_buffer(data)
{
    graphics_buffer = gpu->create_mesh(data);
}

#ifdef KL_USING_PHYSX

kl::mesh::mesh(scene* scene, gpu* gpu, const mesh_data& data)
    : mesh(gpu, data)
{
    PxTriangleMeshDesc mesh_descriptor = {};
    mesh_descriptor.points.count = (PxU32) data.size();
    mesh_descriptor.points.stride = (PxU32) sizeof(vertex);
    mesh_descriptor.points.data = data.data();

    PxDefaultMemoryOutputStream cook_buffer = {};
    scene->get_cooking()->cookTriangleMesh(mesh_descriptor, cook_buffer);

    PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
    physics_buffer = scene->get_physics()->createTriangleMesh(cooked_buffer);
    error_check(!physics_buffer, "Failed to create triangle mesh shape");
}

kl::mesh::~mesh()
{
    physics_buffer->release();
}

#endif
