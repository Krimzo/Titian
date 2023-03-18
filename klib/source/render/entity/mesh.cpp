#include "render/entity/mesh.h"

#include "render/render.h"


#ifdef KL_USING_PHYSX

kl::mesh::mesh(ref<gpu> gpu, ref<scene> scene, const mesh_data& vertices)
    : gpu_(gpu), scene_(scene), buffer(vertices)
{
    // Graphics
    graphics_buffer = gpu->create_mesh(vertices);

    // Physics
    PxTriangleMeshDesc mesh_descriptor = {};
    mesh_descriptor.points.count = (PxU32) vertices.size();
    mesh_descriptor.points.stride = (PxU32) sizeof(vertex);
    mesh_descriptor.points.data = vertices.data();

    PxDefaultMemoryOutputStream cook_buffer = {};
    scene->get_cooking()->cookTriangleMesh(mesh_descriptor, cook_buffer);

    PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
    physics_buffer = scene->get_physics()->createTriangleMesh(cooked_buffer);
}

kl::mesh::~mesh()
{}

#endif
