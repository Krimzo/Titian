#pragma once

#include "serialization/serialization.h"


namespace titian {
    struct Vertex
    {
        Float3 world;
        Float2 texture;
        Float3 normal;
        kl::Vector4<byte> bone_indices;
        Float4 bone_weights;
    };
}

namespace titian {
    struct SkeletonNode
    {
        int bone_index = -1;
		Float4x4 transformation;
		Vector<Ref<SkeletonNode>> children;
    };
}

namespace titian {
    struct Scene;
}

namespace titian {
    struct Mesh : Serializable
    {
        int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        bool render_wireframe = false;

        Vector<Vertex> data_buffer;
        dx::Buffer graphics_buffer;

        Vector<Float4x4> bone_matrices;
        Ref<SkeletonNode> skeleton_root;

        Mesh();

        void serialize(Serializer& serializer) const override;
        void deserialize(const Serializer& serializer) override;

        D3D_PRIMITIVE_TOPOLOGY casted_topology() const;

        void load_vertices(const Vector<kl::Vertex<float>>& vertices);
        void load_triangles(const Vector<kl::Triangle<float>>& triangles);
        void reload();
    };
}
