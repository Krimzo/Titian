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
    serializer->write_int("topology", topology);
    serializer->write_bool("render_wireframe", render_wireframe);

    serializer->write_int("data_buffer_size", (int32_t) data_buffer.size());
    int counter = 0;
    for (const auto& data : data_buffer) {
		serializer->write_byte_array(kl::format("__vertex_", counter), &data, sizeof(Vertex));
        counter += 1;
    }

    serializer->write_int("bone_matrices_size", (int32_t) bone_matrices.size());
    counter = 0;
    for (const auto& mat : bone_matrices) {
        serializer->write_float_array(kl::format("__bone_matrix_", counter), mat.data, 16);
        counter += 1;
    }

    Function<void(const SkeletonNode*)> rec_helper;
    counter = 0;
    rec_helper = [&](const SkeletonNode* node)
    {
        serializer->push_object(kl::format("__node_", counter++));
        serializer->write_int("bone_index", node->bone_index);
        serializer->write_float_array("transformation", node->transformation.data, 16);
        serializer->write_int("children_size", (int32_t) node->children.size());
        for (auto& child : node->children) {
			rec_helper(&child);
        }
        serializer->pop_object();
    };

    serializer->write_bool("has_data", (bool) skeleton_root);
    if (skeleton_root) {
		rec_helper(&skeleton_root);
	}
}

void titian::Mesh::deserialize(const Serializer* serializer, const void* helper_data)
{
    serializer->read_int("topology", topology);
    serializer->read_bool("render_wireframe", render_wireframe);

    int32_t data_buffer_size = 0;
    serializer->read_int("data_buffer_size", data_buffer_size);
    data_buffer.resize(data_buffer_size);
    int counter = 0;
    for (auto& data : data_buffer) {
        serializer->read_byte_array(kl::format("__vertex_", counter), &data, sizeof(Vertex));
        counter += 1;
    }

    int32_t bone_matrices_size = 0;
    serializer->read_int("bone_matrices_size", bone_matrices_size);
    bone_matrices.resize(bone_matrices_size);
    counter = 0;
    for (auto& mat : bone_matrices) {
        serializer->read_float_array(kl::format("__bone_matrix_", counter), mat.data, 16);
        counter += 1;
    }

    Function<void(SkeletonNode*)> rec_helper;
    counter = 0;
    rec_helper = [&](SkeletonNode* node)
    {
        serializer->load_object(kl::format("__node_", counter++));
        serializer->read_int("bone_index", node->bone_index);
        serializer->read_float_array("transformation", node->transformation.data, 16);
        int32_t children_size = 0;
        serializer->read_int("children_size", children_size);
        node->children.resize(children_size);
        for (auto& child : node->children) {
			child = new SkeletonNode();
            rec_helper(&child);
        }
        serializer->unload_object();
    };

    bool has_skeleton = false;
    serializer->read_bool("has_data", has_skeleton);
    if (has_skeleton) {
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
