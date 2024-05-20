#pragma once

#include "serialization/serializable.h"
#include "physx.h"


namespace titian {
    class Mesh : public Serializable
    {
    public:
        using Data = std::vector<kl::Vertex<float>>;

        Data data_buffer = {};
        kl::dx::Buffer graphics_buffer = nullptr;
        physx::PxTriangleMesh* physics_buffer = nullptr;

        int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        bool render_wireframe = false;

        Mesh(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking);
        ~Mesh() override;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        void reload();
        D3D_PRIMITIVE_TOPOLOGY casted_topology() const;

    private:
        kl::GPU* m_gpu = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;

        void free_physics_buffer();
    };
}
