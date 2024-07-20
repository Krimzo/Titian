#pragma once

#include "serialization/serializable.h"


namespace titian {
    struct Vertex
    {
        kl::Float3 world;
        kl::Float2 texture;
        kl::Float3 normal;
        kl::Vector4<uint8_t> bone_indices;
        kl::Float4 bone_weights;
    };
}

namespace titian {
    struct SkeletonNode
    {
        int bone_index = -1;
		kl::Float4x4 transformation;
		Vector<Ref<SkeletonNode>> children;
    };
}

namespace titian {
    class Mesh : public Serializable
    {
    public:
        using Data = Vector<Vertex>;

        int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        bool render_wireframe = false;

        Data data_buffer = {};
        kl::dx::Buffer graphics_buffer = nullptr;
        px::PxTriangleMesh* physics_buffer = nullptr;

        Vector<kl::Float4x4> bone_matrices;
        Ref<SkeletonNode> skeleton_root;

        Mesh(kl::GPU* gpu, px::PxPhysics* physics, px::PxCooking* cooking);
        ~Mesh() override;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        void load(const Vector<kl::Vertex<float>>& vertices);
        void reload();
        D3D_PRIMITIVE_TOPOLOGY casted_topology() const;

    private:
        kl::GPU* m_gpu = nullptr;
        px::PxPhysics* m_physics = nullptr;
        px::PxCooking* m_cooking = nullptr;

        void free_physics_buffer();
    };
}
