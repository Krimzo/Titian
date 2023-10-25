export module entity;

export import collider;

export namespace titian {
    class Entity : public Serializable
    {
    public:
        enum class Type
        {
            BASIC,
            CAMERA,
            AMBIENT_LIGHT,
            POINT_LIGHT,
            DIRECTIONAL_LIGHT,
        } const type = {};

        kl::Float3 scale{ 1.0f };
        std::string mesh_name = "/";
        std::string material_name = "/";

        // Creation
        Entity(const Type type, physx::PxPhysics* physics, const bool dynamic)
            : type(type), m_physics(physics)
        {
            physx::PxTransform transform = {};
            transform.q = physx::PxQuat(physx::PxIdentity);
            transform.p = physx::PxVec3(physx::PxZero);
            generate_actor(transform, dynamic);
        }

        ~Entity() override
        {
            cleanup();
        }

        Entity(const Entity&) = delete;
        Entity(const Entity&&) = delete;

        void operator=(const Entity&) = delete;
        void operator=(const Entity&&) = delete;

        void serialize(Serializer* serializer, const void* helper_data) const override
        {
            serializer->write_object<Type>(type);

            serializer->write_object<bool>(is_dynamic());
            serializer->write_object<bool>(has_gravity());

            serializer->write_object<float>(mass());

            serializer->write_object<kl::Float3>(scale);
            serializer->write_object<kl::Float3>(rotation());
            serializer->write_object<kl::Float3>(position());
            serializer->write_object<kl::Float3>(velocity());
            serializer->write_object<kl::Float3>(angular());

            serializer->write_string(mesh_name);
            serializer->write_string(material_name);

            const bool has_collider = static_cast<bool>(m_collider);
            serializer->write_object<bool>(has_collider);
            if (has_collider) {
                m_collider->serialize(serializer, helper_data);
            }
        }

        void deserialize(const Serializer* serializer, const void* helper_data) override
        {
            set_dynamic(serializer->read_object<bool>());
            set_gravity(serializer->read_object<bool>());

            set_mass(serializer->read_object<float>());

            serializer->read_object<kl::Float3>(scale);
            set_rotation(serializer->read_object<kl::Float3>());
            set_position(serializer->read_object<kl::Float3>());
            set_velocity(serializer->read_object<kl::Float3>());
            set_angular(serializer->read_object<kl::Float3>());

            serializer->read_string(mesh_name);
            serializer->read_string(material_name);

            const bool has_collider = serializer->read_object<bool>();
            if (has_collider) {
                kl::Object new_collider = new Collider(m_physics);
                new_collider->deserialize(serializer, helper_data);
                this->set_collider(new_collider);
            }
        }

        // Get
        physx::PxRigidActor* actor() const
        {
            return m_actor;
        }

        kl::Float4x4 model_matrix() const
        {
            return kl::Float4x4::translation(position()) * kl::Float4x4::rotation(rotation()) * kl::Float4x4::scaling(scale);
        }

        kl::Float4x4 collider_matrix() const
        {
            if (!m_collider) {
                return {};
            }

            kl::Float4x4 result = kl::Float4x4::translation(position());
            result *= kl::Float4x4::rotation(rotation());
            result *= kl::Float4x4::translation(m_collider->offset());
            result *= kl::Float4x4::rotation(m_collider->rotation());
            result *= m_collider->scaling_matrix();
            return result;
        }

        // Geometry
        void set_rotation(const kl::Float3& rotation)
        {
            const kl::Float4 quat = kl::to_quaternion(rotation);
            physx::PxTransform transform = m_actor->getGlobalPose();
            transform.q = (const physx::PxQuat&) quat;
            m_actor->setGlobalPose(transform);
        }

        kl::Float3 rotation() const
        {
            const physx::PxTransform transform = m_actor->getGlobalPose();
            return kl::to_euler((const kl::Float4&) transform.q);
        }

        void set_position(const kl::Float3& position)
        {
            physx::PxTransform transform = m_actor->getGlobalPose();
            transform.p = (const physx::PxVec3&) position;
            m_actor->setGlobalPose(transform);
        }

        kl::Float3 position() const
        {
            const physx::PxTransform transform = m_actor->getGlobalPose();
            return (const kl::Float3&) transform.p;
        }

        // Physics
        void set_dynamic(const bool enabled)
        {
            const bool old_dynamic = is_dynamic();
            if ((old_dynamic && enabled) || (!old_dynamic && !enabled)) {
                return;
            }
            const physx::PxTransform old_transform = m_actor->getGlobalPose();
            kl::Object<Collider> old_collider = m_collider;
            set_collider(nullptr);
            generate_actor(old_transform, enabled);
            set_collider(old_collider);
        }

        bool is_dynamic() const
        {
            const physx::PxActorType::Enum type = m_actor->getType();
            return type == physx::PxActorType::Enum::eRIGID_DYNAMIC;
        }

        void set_gravity(const bool enabled)
        {
            m_actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
            if (enabled) {
                wake_up();
            }
        }

        bool has_gravity() const
        {
            const physx::PxActorFlags flags = m_actor->getActorFlags();
            return !flags.isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
        }

        void set_mass(const float mass)
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            actor->setMass(mass);
        }

        float mass() const
        {
            if (!is_dynamic()) {
                return 0.0f;
            }
            const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            return actor->getMass();
        }

        void set_velocity(const kl::Float3& velocity)
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            actor->setLinearVelocity((const physx::PxVec3&) velocity);
        }

        kl::Float3 velocity() const
        {
            if (!is_dynamic()) {
                return {};
            }
            const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            const physx::PxVec3 velocity = actor->getLinearVelocity();
            return (const kl::Float3&) velocity;
        }

        void set_angular(const kl::Float3& angular)
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            actor->setAngularVelocity((const physx::PxVec3&) angular);
        }

        kl::Float3 angular() const
        {
            if (!is_dynamic()) {
                return {};
            }
            const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            const physx::PxVec3 angular = actor->getAngularVelocity();
            return (const kl::Float3&) angular;
        }

        // Collision
        void set_collider(const kl::Object<Collider>& collider)
        {
            if (m_collider) {
                m_actor->detachShape(*m_collider->shape());
            }
            if (collider) {
                m_actor->attachShape(*collider->shape());
            }
            m_collider = collider;
        }

        kl::Object<Collider> collider() const
        {
            return m_collider;
        }

    private:
        physx::PxPhysics* m_physics = nullptr;
        physx::PxRigidActor* m_actor = nullptr;
        kl::Object<Collider> m_collider = nullptr;

        void cleanup()
        {
            if (m_actor) {
                m_actor->release();
                m_actor = nullptr;
            }
        }

        void generate_actor(const physx::PxTransform& transform, const bool dynamic)
        {
            cleanup();
            if (dynamic) {
                m_actor = m_physics->createRigidDynamic(transform);
                set_mass(1.0f);
                set_gravity(false);
            }
            else {
                m_actor = m_physics->createRigidStatic(transform);
            }
            kl::assert(m_actor, "Failed to create physics actor");
        }

        void wake_up() const
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_actor;
            if (actor->getScene()) {
                actor->wakeUp();
            }
        }
    };
}
