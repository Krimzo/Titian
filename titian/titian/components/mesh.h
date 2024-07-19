#pragma once

#include "serialization/serializable.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "assimp/quaternion.h"
#include "assimp/scene.h"
#include "physx.h"

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "zlibstaticd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#pragma comment(lib, "zlibstatic.lib")
#endif


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
		std::vector<kl::Object<SkeletonNode>> children;
    };
}

namespace titian {
    class Mesh : public Serializable
    {
    public:
        using Data = std::vector<Vertex>;

        int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        bool render_wireframe = false;

        Data data_buffer = {};
        kl::dx::Buffer graphics_buffer = nullptr;
        physx::PxTriangleMesh* physics_buffer = nullptr;

        std::vector<kl::Float4x4> bone_matrices;
        kl::Object<SkeletonNode> skeleton_root;

        Mesh(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking);
        ~Mesh() override;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        void load(const std::vector<kl::Vertex<float>>& vertices);
        void reload();
        D3D_PRIMITIVE_TOPOLOGY casted_topology() const;

    private:
        kl::GPU* m_gpu = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;

        void free_physics_buffer();
    };
}
