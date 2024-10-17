#pragma once

#include "serial/serial.h"


namespace titian {
    using Byte4 = kl::Vector4<byte>;
}

namespace titian {
    struct Vertex : kl::Vertex
    {
        Byte4 bone_indices;
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
        D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        bool render_wireframe = false;

        Vector<Vertex> vertices;
        dx::Buffer buffer;

        Vector<Float4x4> bone_matrices;
        Ref<SkeletonNode> skeleton_root;

        Mesh();

        void serialize(Serializer& serializer) const override;
        void deserialize(const Serializer& serializer) override;

        void load_vertices(const Vector<kl::Vertex>& vertices);
        void load_triangles(const Vector<kl::Triangle>& triangles);
        void reload();

        Ref<Mesh> clone() const;
    };
}
