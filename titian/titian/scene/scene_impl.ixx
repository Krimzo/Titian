export module scene_impl;

export import scene;

export import native_script;
export import interpreted_script;
export import node_script;

export import camera;
export import ambient_light;
export import point_light;
export import directional_light;

physx::PxDefaultAllocator titian::Scene::m_allocator = {};
physx::PxDefaultErrorCallback titian::Scene::m_error_callback = {};
physx::PxFoundation* titian::Scene::m_foundation = PxCreateFoundation(PX_VERSION, m_allocator, m_error_callback);

void titian::Scene::deserialize(const Serializer* serializer, const void* helper_data)
{
    serializer->read_string(main_camera_name);
    serializer->read_string(main_ambient_light_name);
    serializer->read_string(main_directional_light_name);

    auto read_map = [&]<typename T>(std::map<std::string, T>& data, const std::function<T()>& provider, const void* helper_data)
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            kl::Object object = provider();
            object->deserialize(serializer, helper_data);
            data[name] = object;
        }
    };

    std::function mesh_provider = [&] { return kl::Object{ new Mesh(m_gpu, m_physics, m_cooking) }; };
    read_map(meshes, mesh_provider, nullptr);

    std::function texture_provider = [&] { return kl::Object{ new Texture(m_gpu) }; };
    read_map(textures, texture_provider, nullptr);

    std::function material_provider = [&] { return kl::Object{ new Material() }; };
    read_map(materials, material_provider, nullptr);

    /* SCRIPTS */
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            const Script::Type type = serializer->read_object<Script::Type>();

            kl::Object<Script> object = nullptr;
            switch (type) {
            case Script::Type::NATIVE:
                object = new NativeScript();
                break;
            case Script::Type::INTERPRETED:
                object = new InterpretedScript();
                break;
            case Script::Type::NODE:
                object = new NodeScript();
                break;
            }

            object->deserialize(serializer, nullptr);
            scripts[name] = object;
        }
    }

    /* ENTITIES */
    {
        const uint64_t size = serializer->read_object<uint64_t>();
        for (uint64_t i = 0; i < size; i++) {
            const std::string name = serializer->read_string();
            const Entity::Type type = serializer->read_object<Entity::Type>();

            kl::Object<Entity> entity = nullptr;
            switch (type) {
            case Entity::Type::BASIC:
                entity = new Entity(Entity::Type::BASIC, m_physics, false);
                break;
            case Entity::Type::CAMERA:
                entity = new Camera(m_physics, false);
                break;
            case Entity::Type::AMBIENT_LIGHT:
                entity = new AmbientLight(m_physics, false);
                break;
            case Entity::Type::POINT_LIGHT:
                entity = new PointLight(m_physics, false);
                break;
            case Entity::Type::DIRECTIONAL_LIGHT:
                entity = new DirectionalLight(m_physics, false, m_gpu, 4096);
                break;
            }

            entity->deserialize(serializer, &meshes);
            this->add_entity(name, entity);
        }
    }
}
