export module mesh;

export import unique;

export namespace titian {
    class Mesh : public Unique
    {
    public:
        using Data = std::vector<kl::Vertex>;

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

    public:
        Data data_buffer = {};
        kl::dx::Buffer graphics_buffer = nullptr;
        physx::PxTriangleMesh* physics_buffer = nullptr;

        Mesh(kl::GPU* gpu, physx::PxPhysics* physics, physx::PxCooking* cooking)
            : m_gpu(gpu), m_physics(physics), m_cooking(cooking)
        {}

        ~Mesh() override
        {
            free_physics_buffer();
        }

        void serialize(kl::File* file) const override
        {
            Unique::serialize(file);
            file->write(data_buffer.size());
            file->write(data_buffer.data(), data_buffer.size());
        }
        
        void deserialize(const kl::File* file) override
        {
            Unique::deserialize(file);
            data_buffer.resize(file->read<size_t>());
            file->read(data_buffer.data(), data_buffer.size());
        }

        void reload()
        {
            // Graphics
            graphics_buffer = m_gpu->create_vertex_buffer(data_buffer);

            // Physics
            free_physics_buffer();

            physx::PxTriangleMeshDesc mesh_descriptor = {};
            mesh_descriptor.points.count = (physx::PxU32) data_buffer.size();
            mesh_descriptor.points.stride = (physx::PxU32) sizeof(Data::value_type);
            mesh_descriptor.points.data = data_buffer.data();

            physx::PxDefaultMemoryOutputStream cook_buffer = {};
            m_cooking->cookTriangleMesh(mesh_descriptor, cook_buffer);

            physx::PxDefaultMemoryInputData cooked_buffer(cook_buffer.getData(), cook_buffer.getSize());
            physics_buffer = m_physics->createTriangleMesh(cooked_buffer);
            kl::assert(physics_buffer, "Failed to create mesh physics buffer");
        }
    };
}
