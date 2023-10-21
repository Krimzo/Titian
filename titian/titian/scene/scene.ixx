export module scene;

export import mesh;
export import texture;
export import material;
export import script;

export import default_meshes;
export import default_materials;

export import native_script;
export import interpreted_script;
export import node_script;

export import camera;
export import ambient_light;
export import point_light;
export import directional_light;

constexpr uint32_t PX_VERSION = 0x4010200;

export namespace titian {
    class Scene : public Serializable
    {
    public:
        std::map<std::string, kl::Object<Mesh>> meshes = {};
        std::map<std::string, kl::Object<Texture>> textures = {};
        std::map<std::string, kl::Object<Material>> materials = {};
        std::map<std::string, kl::Object<Script>> scripts = {};

        kl::Object<DefaultMeshes> default_meshes = nullptr;
        kl::Object<DefaultMaterials> default_materials = nullptr;

        std::string main_camera_name = "/";
        std::string main_ambient_light_name = "/";
        std::string main_directional_light_name = "/";

        Scene(kl::Object<kl::GPU>& gpu)
            : m_gpu(gpu)
        {
            m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
            kl::assert(m_dispatcher, "Failed to create physics dispatcher");

            m_physics = PxCreatePhysics(PX_VERSION, *m_foundation, physx::PxTolerancesScale());
            kl::assert(m_physics, "Failed to create physics");

            m_cooking = PxCreateCooking(PX_VERSION, *m_foundation, physx::PxCookingParams(m_physics->getTolerancesScale()));
            kl::assert(m_cooking, "Failed to create physics cooking");

            physx::PxSceneDesc scene_descriptor = { m_physics->getTolerancesScale() };
            scene_descriptor.gravity.y = -9.81f;
            scene_descriptor.cpuDispatcher = m_dispatcher;
            scene_descriptor.filterShader = physx::PxDefaultSimulationFilterShader;

            m_scene = m_physics->createScene(scene_descriptor);
            kl::assert(m_scene, "Failed to create physics scene");

            default_meshes = new DefaultMeshes(&gpu, m_physics, m_cooking);
            kl::assert(default_meshes, "Failed to init default meshes");
            
            default_materials = new DefaultMaterials();
            kl::assert(default_materials, "Failed to init default materials");
        }

        ~Scene() override
        {
            default_meshes.free();
            default_materials.free();

            meshes.clear();
            textures.clear();
            materials.clear();
            scripts.clear();

            while (!m_entities.empty()) {
                this->remove(m_entities.begin()->first);
            }

            m_scene->release();
            m_dispatcher->release();
            m_cooking->release();
            m_physics->release();
        }

        Scene(const Scene&) = delete;
        Scene(const Scene&&) = delete;

        void operator=(const Scene&) = delete;
        void operator=(const Scene&&) = delete;

        void serialize(Serializer* serializer) const override
        {
            serializer->write_string(main_camera_name);
            serializer->write_string(main_ambient_light_name);
            serializer->write_string(main_directional_light_name);

            auto write_map = [&]<typename T>(const std::map<std::string, kl::Object<T>>& data)
            {
                serializer->write_object<uint64_t>(data.size());
                for (auto& [name, object] : data) {
                    serializer->write_string(name);
                    object->serialize(serializer);
                }
            };
            
            write_map(meshes);
            write_map(textures);
            write_map(materials);
            write_map(scripts);

            Collider::BOUND_MESHES = &meshes;
            write_map(m_entities);
        }
        
        void deserialize(const Serializer* serializer) override
        {
            serializer->read_string(main_camera_name);
            serializer->read_string(main_ambient_light_name);
            serializer->read_string(main_directional_light_name);

            auto read_map = [&]<typename T>(std::map<std::string, T>& data, const std::function<T()>& provider)
            {
                const uint64_t size = serializer->read_object<uint64_t>();
                for (uint64_t i = 0; i < size; i++) {
                    const std::string name = serializer->read_string();
                    kl::Object object = provider();
                    object->deserialize(serializer);
                    data[name] = object;
                }
            };
            
            std::function mesh_provider = [&] { return kl::Object{ new Mesh(&m_gpu, m_physics, m_cooking) }; };
            read_map(meshes, mesh_provider);
            
            std::function texture_provider = [&] { return kl::Object{ new Texture(&m_gpu) }; };
            read_map(textures, texture_provider);

            std::function material_provider = [&] { return kl::Object{ new Material() }; };
            read_map(materials, material_provider);

            /* SCRIPTS */ {
                const uint64_t size = serializer->read_object<uint64_t>();
                for (uint64_t i = 0; i < size; i++) {
                    const std::string name = serializer->read_string();
                    const Script::Type type = serializer->read_object<Script::Type>();

                    kl::Object<Script> object = nullptr;
                    switch (type) {
                    case Script::Type::NATIVE:
                        object = new NativeScript(this);
                        break;
                    case Script::Type::INTERPRETED:
                        object = new InterpretedScript(this);
                        break;
                    case Script::Type::NODE:
                        object = new NodeScript(this);
                        break;
                    }
            
                    object->deserialize(serializer);
                    scripts[name] = object;
                }
            }
            
            Collider::BOUND_MESHES = &meshes;
            /* ENTITIES */ {
                const uint64_t size = serializer->read_object<uint64_t>();
                for (uint64_t i = 0; i < size; i++) {
                    const std::string name = serializer->read_string();
                    const Entity::Type type = serializer->read_object<Entity::Type>();

                    kl::Object<Entity> object = nullptr;
                    switch (type) {
                    case Entity::Type::BASIC:
                        object = new Entity(Entity::Type::BASIC, m_physics, false);
                        break;
                    case Entity::Type::CAMERA:
                        object = new Camera(m_physics, false);
                        break;
                    case Entity::Type::AMBIENT_LIGHT:
                        object = new AmbientLight(m_physics, false);
                        break;
                    case Entity::Type::POINT_LIGHT:
                        object = new PointLight(m_physics, false);
                        break;
                    case Entity::Type::DIRECTIONAL_LIGHT:
                        object = new DirectionalLight(m_physics, false, m_gpu, 4096);
                        break;
                    }
            
                    object->deserialize(serializer);
                    this->add(name, object);
                }
            }
        }

        // Iterate
        std::map<std::string, kl::Object<Entity>>::iterator begin()
        {
            return m_entities.begin();
        }

        std::map<std::string, kl::Object<Entity>>::iterator end()
        {
            return m_entities.end();
        }

        std::map<std::string, kl::Object<Entity>>::const_iterator begin() const
        {
            return m_entities.begin();
        }

        std::map<std::string, kl::Object<Entity>>::const_iterator end() const
        {
            return m_entities.end();
        }

        // Get
        physx::PxPhysics* physics() const
        {
            return m_physics;
        }

        physx::PxCooking* cooking() const
        {
            return m_cooking;
        }

        kl::Object<Mesh> get_mesh(const std::string& id) const
        {
            if (meshes.contains(id)) {
                return meshes.at(id);
            }
            return nullptr;
        }

        kl::Object<Texture> get_texture(const std::string& id) const
        {
            if (textures.contains(id)) {
                return textures.at(id);
            }
            return nullptr;
        }

        kl::Object<Material> get_material(const std::string& id) const
        {
            if (materials.contains(id)) {
                return materials.at(id);
            }
            return nullptr;
        }

        kl::Object<Script> get_script(const std::string& id) const
        {
            if (scripts.contains(id)) {
                return scripts.at(id);
            }
            return nullptr;
        }

        kl::Object<Entity> get_entity(const std::string& id) const
        {
            if (m_entities.contains(id)) {
                return m_entities.at(id);
            }
            return nullptr;
        }

        size_t entity_count() const
        {
            return m_entities.size();
        }

        // Dynamic
        template<typename T>
        T* get_dynamic(const std::string& id)
        {
            kl::Object<Entity> entity = get_entity(id);
            return dynamic_cast<T*>(&entity);
        }

        template<typename T>
        const T* get_dynamic(const std::string& id) const
        {
            const kl::Object<Entity> entity = get_entity(id);
            return dynamic_cast<const T*>(&entity);
        }

        // Set/Get
        void set_gravity(const kl::Float3& gravity)
        {
            m_scene->setGravity((const physx::PxVec3&) gravity);
        }

        kl::Float3 gravity() const
        {
            const physx::PxVec3 gravity = m_scene->getGravity();
            return (const kl::Float3&) gravity;
        }

        void add(const std::string& name, const kl::Object<Entity>& entity)
        {
            m_entities[name] = entity;
            m_scene->addActor(*entity->actor());
        }

        void remove(const std::string& name)
        {
            if (m_entities.contains(name)) {
                m_scene->removeActor(*m_entities.at(name)->actor());
                m_entities.erase(name);
            }
        }

        // Update
        void update_physics(const float delta_t)
        {
            m_scene->simulate(delta_t);
            m_scene->fetchResults(true);
        }

        void update_scripts()
        {
            for (auto& [_, script] : scripts) {
                script->call_update();
            }
        }

        // Entity
        kl::Object<Entity> make_entity(const bool dynamic) const
        {
            return new Entity(Entity::Type::BASIC, m_physics, dynamic);
        }

        // Dynamic colliders
        kl::Object<Collider> make_box_collider(const kl::Float3& scale) const
        {
            return new Collider(m_physics, physx::PxBoxGeometry((physx::PxVec3&) scale));
        }

        kl::Object<Collider> make_sphere_collider(const float radius) const
        {
            return new Collider(m_physics, physx::PxSphereGeometry(radius));
        }

        kl::Object<Collider> make_capsule_collider(const float radius, const float height) const
        {
            return new Collider(m_physics, physx::PxCapsuleGeometry(radius, height));
        }

        // Static colliders
        kl::Object<Collider> make_mesh_collider(const Mesh& mesh, const kl::Float3& scale) const
        {
            return new Collider(m_physics, physx::PxTriangleMeshGeometry(mesh.physics_buffer, (physx::PxVec3&) scale));
        }

        // Default collider
        kl::Object<Collider> make_default_collider(physx::PxGeometryType::Enum type, const Mesh* optional_mesh) const
        {
            switch (type) {
            case physx::PxGeometryType::Enum::eBOX:
                return make_box_collider(kl::Float3{ 1.0f });
            case physx::PxGeometryType::Enum::eSPHERE:
                return make_sphere_collider(1.0f);
            case physx::PxGeometryType::Enum::eCAPSULE:
                return make_capsule_collider(1.0f, 2.0f);

            case physx::PxGeometryType::Enum::eTRIANGLEMESH:
                return make_mesh_collider(*optional_mesh, kl::Float3{ 1.0f });
            }
            return nullptr;
        }

    private:
        static physx::PxDefaultAllocator m_allocator;
        static physx::PxDefaultErrorCallback m_error_callback;
        static physx::PxFoundation* m_foundation;

        physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;
        physx::PxScene* m_scene = nullptr;

        kl::Object<kl::GPU> m_gpu = nullptr;
        std::map<std::string, kl::Object<Entity>> m_entities = {};
    };
}

physx::PxDefaultAllocator titian::Scene::m_allocator = {};
physx::PxDefaultErrorCallback titian::Scene::m_error_callback = {};
physx::PxFoundation* titian::Scene::m_foundation = PxCreateFoundation(PX_VERSION, m_allocator, m_error_callback);
