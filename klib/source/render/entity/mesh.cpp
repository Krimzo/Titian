#include "render/entity/mesh.h"

#include "render/scene.h"


kl::mesh::mesh(gpu* gpu, dx::buffer graphics_buffer)
    : gpu_(gpu), graphics_buffer(graphics_buffer)
{}

kl::mesh::mesh(gpu* gpu, const std::vector<vertex>& vertices)
    : mesh(gpu, gpu->new_vertex_buffer(vertices))
{
    this->vertices = vertices;
}

kl::mesh::mesh(gpu* gpu, scene* scene, const std::vector<vertex>& vertices)
    : mesh(gpu, vertices)
{
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
{
    if (graphics_buffer) {
        gpu_->destroy(graphics_buffer);
        graphics_buffer = nullptr;
    }

    if (physics_buffer) {
        physics_buffer->release();
        physics_buffer = nullptr;
    }
}
