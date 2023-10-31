﻿export module mesh;

export import serializable;

export namespace titian {
    class Mesh : public Serializable
    {
    public:
        using Data = std::vector<kl::Vertex>;

        Data data_buffer = {};
        kl::dx::Buffer graphics_buffer = nullptr;
        physx::PxTriangleMesh* physics_buffer = nullptr;

        int topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        bool render_wireframe = false;

        Mesh(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking)
            : m_gpu(gpu), m_physics(physics), m_cooking(cooking)
        {}

        ~Mesh() override
        {
            free_physics_buffer();
        }

        void serialize(Serializer* serializer, const void* helper_data) const override
        {
            const uint64_t size = data_buffer.size();
            serializer->write_object<uint64_t>(size);

            const Data::value_type* data = data_buffer.data();
            serializer->write_array<Data::value_type>(data, size);

            serializer->write_object<int>(topology);
            serializer->write_object<bool>(render_wireframe);
        }
        
        void deserialize(const Serializer* serializer, const void* helper_data) override
        {
            const uint64_t size = serializer->read_object<uint64_t>();
            data_buffer.resize(size);
            
            Data::value_type* data = data_buffer.data();
            serializer->read_array<Data::value_type>(data, size);

            serializer->read_object<int>(topology);
            serializer->read_object<bool>(render_wireframe);

            this->reload();
        }

        void reload()
        {
            // Empty check
            if (data_buffer.empty()) {
                return;
            }

            // Graphics
            graphics_buffer = m_gpu->create_vertex_buffer(data_buffer);

            // Physics
            free_physics_buffer();

            physx::PxTriangleMeshDesc mesh_descriptor = {};
            mesh_descriptor.points.stride = static_cast<physx::PxU32>(sizeof(Data::value_type));
            mesh_descriptor.points.count = static_cast<physx::PxU32>(data_buffer.size() / 3 * 3);
            mesh_descriptor.points.data = data_buffer.data();

            physx::PxDefaultMemoryOutputStream cook_buffer = {};
            m_cooking->cookTriangleMesh(mesh_descriptor, cook_buffer);

            physx::PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
            physics_buffer = m_physics->createTriangleMesh(cooked_buffer);
        }

        D3D_PRIMITIVE_TOPOLOGY casted_topology() const
        {
            return static_cast<D3D_PRIMITIVE_TOPOLOGY>(topology);
        }

    private:
        kl::GPU* m_gpu = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;

        void free_physics_buffer()
        {
            if (physics_buffer) {
                physics_buffer->release();
                physics_buffer = nullptr;
            }
        }
    };
}
