export module collider;

export import serializable;

export namespace titian {
    class Collider : public Serializable
    {
    public:
        Collider(physx::PxPhysics* physics, const physx::PxGeometry& geometry)
        {
            m_material = physics->createMaterial(0.25f, 0.25f, 0.25f);
            kl::assert(m_material, "Failed to create collider MATERIAL");

            m_shape = physics->createShape(geometry, *m_material);
            kl::assert(m_shape, "Failed to create collider SHAPE");
        }

        ~Collider() override
        {
            m_shape->release();
            m_material->release();
        }

        void serialize(Serializer* serializer) const override
        {

        }

        void deserialize(const Serializer* serializer) override
        {

        }

        // Get
        physx::PxShape* shape() const
        {
            return m_shape;
        }

        physx::PxGeometryType::Enum type() const
        {
            return m_shape->getGeometryType();
        }

        kl::Float4x4 scaling_matrix() const
        {
            switch (type()) {
            case physx::PxGeometryType::Enum::eBOX:
            {
                physx::PxBoxGeometry geometry = {};
                m_shape->getBoxGeometry(geometry);
                return kl::Float4x4::scaling((kl::Float3&) geometry.halfExtents);
            }

            case physx::PxGeometryType::Enum::eSPHERE:
            {
                physx::PxSphereGeometry geometry = {};
                m_shape->getSphereGeometry(geometry);
                return kl::Float4x4::scaling(kl::Float3(geometry.radius));
            }

            case physx::PxGeometryType::Enum::eCAPSULE:
            {
                physx::PxCapsuleGeometry geometry = {};
                m_shape->getCapsuleGeometry(geometry);
                return kl::Float4x4::scaling(kl::Float3(geometry.halfHeight * 0.5f, kl::Float2(geometry.radius)));
            }

            case physx::PxGeometryType::Enum::ePLANE:
            {
                return {};
            }

            case physx::PxGeometryType::Enum::eTRIANGLEMESH:
            {
                physx::PxTriangleMeshGeometry geometry = {};
                m_shape->getTriangleMeshGeometry(geometry);
                return kl::Float4x4::scaling((kl::Float3&) geometry.scale.scale);
            }
            }
            return {};
        }

        // Geometry
        void set_rotation(const kl::Float3& rotation)
        {
            const kl::Float4 quat = kl::to_quaternion(rotation);
            physx::PxTransform transform = m_shape->getLocalPose();
            transform.q = (const physx::PxQuat&) quat;
            m_shape->setLocalPose(transform);
        }

        kl::Float3 rotation() const
        {
            const physx::PxTransform transform = m_shape->getLocalPose();
            return kl::to_euler((const kl::Float4&) transform.q);
        }

        void set_offset(const kl::Float3& position)
        {
            physx::PxTransform transform = m_shape->getLocalPose();
            transform.p = (const physx::PxVec3&) position;
            m_shape->setLocalPose(transform);
        }

        kl::Float3 offset() const
        {
            const physx::PxTransform transform = m_shape->getLocalPose();
            return (const kl::Float3&) transform.p;
        }

        // Material
        void set_static_friction(float friction)
        {
            m_material->setStaticFriction(friction);
        }

        float static_friction() const
        {
            return m_material->getStaticFriction();
        }

        void set_dynamic_friction(float friction)
        {
            m_material->setDynamicFriction(friction);
        }

        float dynamic_friction() const
        {
            return m_material->getDynamicFriction();
        }

        void set_restitution(float restitution)
        {
            m_material->setRestitution(restitution);
        }

        float restitution() const
        {
            return m_material->getRestitution();
        }

    private:
        physx::PxMaterial* m_material = nullptr;
        physx::PxShape* m_shape = nullptr;
    };
}
