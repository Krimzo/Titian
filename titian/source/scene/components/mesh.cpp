#include "titian.h"


titian::Mesh::Mesh()
{}

void titian::Mesh::serialize(Serializer& serializer) const
{
    serializer.write_int("topology", topology);
    serializer.write_bool("render_wireframe", render_wireframe);

    serializer.write_int("data_buffer_size", (int32_t) data_buffer.size());
    int counter = 0;
    for (const auto& data : data_buffer) {
		serializer.write_byte_array(kl::format("__vertex_", counter), &data, sizeof(Vertex));
        counter += 1;
    }

    serializer.write_int("bone_matrices_size", (int32_t) bone_matrices.size());
    counter = 0;
    for (const auto& mat : bone_matrices) {
        serializer.write_float_array(kl::format("__bone_matrix_", counter), mat.data, 16);
        counter += 1;
    }

    Function<void(const SkeletonNode*)> rec_helper;
    counter = 0;
    rec_helper = [&](const SkeletonNode* node)
    {
        serializer.push_object(kl::format("__node_", counter++));
        serializer.write_int("bone_index", node->bone_index);
        serializer.write_float_array("transformation", node->transformation.data, 16);
        serializer.write_int("children_size", (int32_t) node->children.size());
        for (auto& child : node->children) {
			rec_helper(&child);
        }
        serializer.pop_object();
    };

    serializer.write_bool("has_data", (bool) skeleton_root);
    if (skeleton_root) {
		rec_helper(&skeleton_root);
	}
}

void titian::Mesh::deserialize(const Serializer& serializer)
{
    serializer.read_int("topology", (int32_t&) topology);
    serializer.read_bool("render_wireframe", render_wireframe);

    int32_t data_buffer_size = 0;
    serializer.read_int("data_buffer_size", data_buffer_size);
    data_buffer.resize(data_buffer_size);
    int counter = 0;
    for (auto& data : data_buffer) {
        serializer.read_byte_array(kl::format("__vertex_", counter), &data, sizeof(Vertex));
        counter += 1;
    }

    int32_t bone_matrices_size = 0;
    serializer.read_int("bone_matrices_size", bone_matrices_size);
    bone_matrices.resize(bone_matrices_size);
    counter = 0;
    for (auto& mat : bone_matrices) {
        serializer.read_float_array(kl::format("__bone_matrix_", counter), mat.data, 16);
        counter += 1;
    }

    Function<void(SkeletonNode*)> rec_helper;
    counter = 0;
    rec_helper = [&](SkeletonNode* node)
    {
        serializer.load_object(kl::format("__node_", counter++));
        serializer.read_int("bone_index", node->bone_index);
        serializer.read_float_array("transformation", node->transformation.data, 16);
        int32_t children_size = 0;
        serializer.read_int("children_size", children_size);
        node->children.resize(children_size);
        for (auto& child : node->children) {
			child = new SkeletonNode();
            rec_helper(&child);
        }
        serializer.unload_object();
    };

    bool has_skeleton = false;
    serializer.read_bool("has_data", has_skeleton);
    if (has_skeleton) {
		skeleton_root = new SkeletonNode();
        rec_helper(&skeleton_root);
    }

    this->reload();
}

void titian::Mesh::load_vertices(const Vector<kl::Vertex<float>>& vertices)
{
	data_buffer.resize(vertices.size());
    for (size_t i = 0; i < vertices.size(); i++) {
        reinterpret_cast<kl::Vertex<float>&>(data_buffer[i]) = vertices[i];
    }
    reload();
}

void titian::Mesh::load_triangles(const Vector<kl::Triangle<float>>& triangles)
{
    data_buffer.resize(triangles.size() * 3);
    for (size_t i = 0; i < triangles.size(); i++) {
        reinterpret_cast<kl::Vertex<float>&>(data_buffer[i * 3 + 0]) = triangles[i].a;
        reinterpret_cast<kl::Vertex<float>&>(data_buffer[i * 3 + 1]) = triangles[i].b;
        reinterpret_cast<kl::Vertex<float>&>(data_buffer[i * 3 + 2]) = triangles[i].c;
    }
    reload();
}

void titian::Mesh::reload()
{
    graphics_buffer = {};
    if (data_buffer.empty()) {
        return;
    }
    kl::GPU& gpu = Layers::get<AppLayer>().gpu;
    graphics_buffer = gpu.create_vertex_buffer(data_buffer.data(), UINT(data_buffer.size() * sizeof(Vertex)));
}

titian::Ref<titian::Mesh> titian::Mesh::clone() const
{
    Ref mesh = new Mesh();
    mesh->topology = topology;
    mesh->render_wireframe = render_wireframe;
    mesh->data_buffer = data_buffer;
    // mesh->graphics_buffer <- reload()
    mesh->bone_matrices = bone_matrices;

    Function<void(const SkeletonNode*, SkeletonNode*)> rec_helper;
    rec_helper = [&](const SkeletonNode* source, SkeletonNode* target)
    {
        if (!source || !target)
            return;
        target->bone_index = source->bone_index;
		target->transformation = source->transformation;
        target->children.resize(source->children.size());
        for (int i = 0; i < source->children.size(); i++) {
            target->children[i] = new SkeletonNode();
			rec_helper(&source->children[i], &target->children[i]);
        }
    };
    rec_helper(&skeleton_root, &mesh->skeleton_root);

    mesh->reload();
    return mesh;
}
