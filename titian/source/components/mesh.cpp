#include "titian.h"


titian::Mesh::Mesh(kl::GPU* gpu, px::PxPhysics* physics, px::PxCooking* cooking)
    : m_gpu(gpu), m_physics(physics), m_cooking(cooking)
{}

titian::Mesh::~Mesh()
{
    free_physics_buffer();
}

void titian::Mesh::serialize(Serializer* serializer, const void* helper_data) const
{
    serializer->write_object<int>(topology);
    serializer->write_object<bool>(render_wireframe);

    serializer->write_object<uint64_t>(data_buffer.size());
    for (auto& data : data_buffer) {
		serializer->write_object(data);
    }

    serializer->write_object<uint64_t>(bone_matrices.size());
    for (auto& mat : bone_matrices) {
        serializer->write_object(mat);
    }

    Function<void(const SkeletonNode*)> rec_helper;
    rec_helper = [&](const SkeletonNode* node)
    {
        serializer->write_object<int>(node->bone_index);
        serializer->write_object<Float4x4>(node->transformation);
        serializer->write_object<uint64_t>(node->children.size());
        for (auto& child : node->children) {
			rec_helper(&child);
        }
    };

    serializer->write_object<bool>((bool) skeleton_root);
    if (skeleton_root) {
		rec_helper(&skeleton_root);
	}
}

void titian::Mesh::deserialize(const Serializer* serializer, const void* helper_data)
{
    serializer->read_object<int>(topology);
    serializer->read_object<bool>(render_wireframe);

    data_buffer.resize(serializer->read_object<uint64_t>());
    for (auto& data : data_buffer) {
        serializer->read_object(data);
    }

    bone_matrices.resize(serializer->read_object<uint64_t>());
    for (auto& mat : bone_matrices) {
        serializer->read_object(mat);
    }

    Function<void(SkeletonNode*)> rec_helper;
    rec_helper = [&](SkeletonNode* node)
    {
        serializer->read_object<int>(node->bone_index);
        serializer->read_object<Float4x4>(node->transformation);
        node->children.resize(serializer->read_object<uint64_t>());
        for (auto& child : node->children) {
			child = new SkeletonNode();
            rec_helper(&child);
        }
    };

    if (serializer->read_object<bool>()) {
		skeleton_root = new SkeletonNode();
        rec_helper(&skeleton_root);
    }

    this->reload();
}

void titian::Mesh::load(const Vector<kl::Vertex<float>>& vertices)
{
	data_buffer.resize(vertices.size());
    for (size_t i = 0; i < vertices.size(); i++) {
        data_buffer[i].world = vertices[i].world;
        data_buffer[i].texture = vertices[i].texture;
		data_buffer[i].normal = vertices[i].normal;
    }
}

void titian::Mesh::reload()
{
    // Empty check
    if (data_buffer.empty()) {
        return;
    }

    // Graphics
    graphics_buffer = m_gpu->create_vertex_buffer(data_buffer.data(), (UINT) (data_buffer.size() * sizeof(Data::value_type)));

    // Physics
    free_physics_buffer();

    px::PxTriangleMeshDesc mesh_descriptor = {};
    mesh_descriptor.points.stride = static_cast<px::PxU32>(sizeof(Data::value_type));
    mesh_descriptor.points.count = static_cast<px::PxU32>(data_buffer.size() / 3 * 3);
    mesh_descriptor.points.data = data_buffer.data();

    px::PxDefaultMemoryOutputStream cook_buffer = {};
    m_cooking->cookTriangleMesh(mesh_descriptor, cook_buffer);

    px::PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
    physics_buffer = m_physics->createTriangleMesh(cooked_buffer);
}

D3D_PRIMITIVE_TOPOLOGY titian::Mesh::casted_topology() const
{
    return static_cast<D3D_PRIMITIVE_TOPOLOGY>(topology);
}

void titian::Mesh::free_physics_buffer()
{
    if (physics_buffer) {
        physics_buffer->release();
        physics_buffer = nullptr;
    }
}
