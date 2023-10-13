export module entity;

export import collider;

export namespace titian {
    class Entity : public Unique
    {
    public:
        kl::Float3 scale{ 1.0f };
        std::string mesh = "/";
        std::string material = "/";

        // Creation
        Entity(physx::PxPhysics* physics, const bool dynamic)
            : m_physics(physics)
        {
            physx::PxTransform transform = {};
            transform.q = physx::PxQuat(physx::PxIdentity);
            transform.p = physx::PxVec3(physx::PxZero);
            generate_actor(transform, dynamic);
        }

        ~Entity() override
        {
            m_physics_actor->release();
        }

        void serialize(Serializer* serializer) const override
        {
            Unique::serialize(serializer);

            serializer->write_object(scale);
            serializer->write_object(rotation());
            serializer->write_object(position());
            serializer->write_object(is_dynamic());
            serializer->write_object(has_gravity());
            serializer->write_object(mass());
            serializer->write_object(velocity());
            serializer->write_object(angular());

            serializer->write_string(mesh);
            serializer->write_string(material);
        }

        void deserialize(const Serializer* serializer) override
        {
            Unique::deserialize(serializer);

            serializer->read_object(scale);
            set_rotation(serializer->read_object<kl::Float3>());
            set_position(serializer->read_object<kl::Float3>());
            set_dynamic(serializer->read_object<bool>());
            set_gravity(serializer->read_object<bool>());
            set_mass(serializer->read_object<float>());
            set_velocity(serializer->read_object<kl::Float3>());
            set_angular(serializer->read_object<kl::Float3>());

            serializer->read_string(mesh);
            serializer->read_string(material);
        }

        // Get
        physx::PxRigidActor* actor() const
        {
            return m_physics_actor;
        }

        kl::Float4x4 matrix() const
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
            physx::PxTransform transform = m_physics_actor->getGlobalPose();
            transform.q = (const physx::PxQuat&) quat;
            m_physics_actor->setGlobalPose(transform);
        }

        kl::Float3 rotation() const
        {
            const physx::PxTransform transform = m_physics_actor->getGlobalPose();
            return kl::to_euler((const kl::Float4&) transform.q);
        }

        void set_position(const kl::Float3& position)
        {
            physx::PxTransform transform = m_physics_actor->getGlobalPose();
            transform.p = (const physx::PxVec3&) position;
            m_physics_actor->setGlobalPose(transform);
        }

        kl::Float3 position() const
        {
            const physx::PxTransform transform = m_physics_actor->getGlobalPose();
            return (const kl::Float3&) transform.p;
        }

        // Physics
        void set_dynamic(const bool enabled)
        {
            const bool old_dynamic = is_dynamic();
            if ((old_dynamic && enabled) || (!old_dynamic && !enabled)) {
                return;
            }
            const physx::PxTransform old_transform = m_physics_actor->getGlobalPose();
            const kl::Object<Collider> old_collider = m_collider;
            set_collider(nullptr);
            generate_actor(old_transform, enabled);
            set_collider(old_collider);
        }

        bool is_dynamic() const
        {
            return m_physics_actor->getType() == physx::PxActorType::Enum::eRIGID_DYNAMIC;
        }

        void set_gravity(const bool enabled)
        {
            m_physics_actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
            if (enabled) {
                wake_up();
            }
        }

        bool has_gravity() const
        {
            const physx::PxActorFlags flags = m_physics_actor->getActorFlags();
            return !flags.isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
        }

        void set_mass(const float mass)
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            actor->setMass(mass);
        }

        float mass() const
        {
            if (!is_dynamic()) {
                return 0.0f;
            }
            const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            return actor->getMass();
        }

        void set_velocity(const kl::Float3& velocity)
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            actor->setLinearVelocity((const physx::PxVec3&) velocity);
        }

        kl::Float3 velocity() const
        {
            if (!is_dynamic()) {
                return {};
            }
            const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            const physx::PxVec3 velocity = actor->getLinearVelocity();
            return (const kl::Float3&) velocity;
        }

        void set_angular(const kl::Float3& angular)
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            actor->setAngularVelocity((const physx::PxVec3&) angular);
        }

        kl::Float3 angular() const
        {
            if (!is_dynamic()) {
                return {};
            }
            const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            const physx::PxVec3 angular = actor->getAngularVelocity();
            return (const kl::Float3&) angular;
        }

        // Collision
        void set_collider(const kl::Object<Collider>& collider)
        {
            if (m_collider) {
                m_physics_actor->detachShape(*m_collider->shape());
            }
            m_collider = collider;
            if (m_collider) {
                m_physics_actor->attachShape(*m_collider->shape());
            }
        }

        kl::Object<Collider> collider() const
        {
            return m_collider;
        }

    private:
        physx::PxPhysics* m_physics = nullptr;
        physx::PxRigidActor* m_physics_actor = nullptr;
        kl::Object<Collider> m_collider = nullptr;

        void generate_actor(const physx::PxTransform& transform, bool dynamic)
        {
            if (m_physics_actor) {
                m_physics_actor->release();
                m_physics_actor = nullptr;
            }
            m_physics_actor = dynamic ? m_physics->createRigidDynamic(transform) : (physx::PxRigidActor*) m_physics->createRigidStatic(transform);
            kl::assert(m_physics_actor, "Failed to create physics actor");

            if (dynamic) {
                set_mass(1.0f);
                set_gravity(false);
            }
        }

        void wake_up() const
        {
            if (!is_dynamic()) {
                return;
            }
            physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
            if (actor->getScene()) {
                actor->wakeUp();
            }
        }
    };
}
