export module collider;

export import mesh;

export namespace titian {
    class Collider : public Serializable
    {
    public:
        Collider(physx::PxPhysics* physics)
            : m_physics(physics)
        {
            m_material = m_physics->createMaterial(0.25f, 0.25f, 0.25f);
            kl::assert(m_material, "Failed to create collider MATERIAL");
        }

        Collider(physx::PxPhysics* physics, const physx::PxGeometry& geometry)
            : Collider(physics)
        {
            set_geometry(geometry);
        }

        ~Collider() override
        {
            if (m_shape) {
                m_shape->release();
                m_shape = nullptr;
            }
            if (m_material) {
                m_material->release();
                m_material = nullptr;
            }
        }

        void serialize(Serializer* serializer, const void* helper_data) const override
        {
            auto helper_meshes = static_cast<const std::map<std::string, kl::Object<Mesh>>*>(helper_data);

            const physx::PxGeometryType::Enum type = this->type();
            serializer->write_object<physx::PxGeometryType::Enum>(type);

            switch (type) {
                case physx::PxGeometryType::Enum::eBOX: {
                    physx::PxBoxGeometry geometry = {};
                    m_shape->getBoxGeometry(geometry);
                    serializer->write_object<kl::Float3>(reinterpret_cast<kl::Float3&>(geometry.halfExtents));
                    break;
                }
                case physx::PxGeometryType::Enum::eSPHERE: {
                    physx::PxSphereGeometry geometry = {};
                    m_shape->getSphereGeometry(geometry);
                    serializer->write_object<float>(geometry.radius);
                    break;
                }
                case physx::PxGeometryType::Enum::eCAPSULE: {
                    physx::PxCapsuleGeometry geometry = {};
                    m_shape->getCapsuleGeometry(geometry);
                    serializer->write_object<float>(geometry.halfHeight);
                    serializer->write_object<float>(geometry.radius);
                    break;
                }
                case physx::PxGeometryType::Enum::eTRIANGLEMESH: {
                    physx::PxTriangleMeshGeometry geometry = {};
                    m_shape->getTriangleMeshGeometry(geometry);
                    serializer->write_object<kl::Float3>(reinterpret_cast<kl::Float3&>(geometry.scale.scale));
                    bool was_found = false;
                    for (const auto& [name, mesh] : *helper_meshes) {
                        if (mesh->physics_buffer == geometry.triangleMesh) {
                            serializer->write_string(name);
                            was_found = true;
                            break;
                        }
                    }
                    kl::assert(was_found, "Failed to find collider MESH");
                    break;
                }
            }

            serializer->write_object<kl::Float3>(rotation());
            serializer->write_object<kl::Float3>(offset());

            serializer->write_object<float>(static_friction());
            serializer->write_object<float>(dynamic_friction());
            serializer->write_object<float>(restitution());
        }

        void deserialize(const Serializer* serializer, const void* helper_data) override
        {
            auto helper_meshes = static_cast<const std::map<std::string, kl::Object<Mesh>>*>(helper_data);

            const physx::PxGeometryType::Enum type = serializer->read_object<physx::PxGeometryType::Enum>();

            switch (type) {
                case physx::PxGeometryType::Enum::eBOX: {
                    physx::PxBoxGeometry geometry = {};
                    serializer->read_object<kl::Float3>(reinterpret_cast<kl::Float3&>(geometry.halfExtents));
                    this->set_geometry(geometry);
                    break;
                }
                case physx::PxGeometryType::Enum::eSPHERE: {
                    physx::PxSphereGeometry geometry = {};
                    serializer->read_object<float>(geometry.radius);
                    this->set_geometry(geometry);
                    break;
                }
                case physx::PxGeometryType::Enum::eCAPSULE: {
                    physx::PxCapsuleGeometry geometry = {};
                    serializer->read_object<float>(geometry.halfHeight);
                    serializer->read_object<float>(geometry.radius);
                    this->set_geometry(geometry);
                    break;
                }
                case physx::PxGeometryType::Enum::eTRIANGLEMESH: {
                    physx::PxTriangleMeshGeometry geometry = {};
                    serializer->read_object<kl::Float3>(reinterpret_cast<kl::Float3&>(geometry.scale.scale));
                    geometry.triangleMesh = (*helper_meshes).at(serializer->read_string())->physics_buffer;
                    this->set_geometry(geometry);
                    break;
                }
            }

            set_rotation(serializer->read_object<kl::Float3>());
            set_offset(serializer->read_object<kl::Float3>());

            set_static_friction(serializer->read_object<float>());
            set_dynamic_friction(serializer->read_object<float>());
            set_restitution(serializer->read_object<float>());
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
                case physx::PxGeometryType::Enum::eBOX: {
                    physx::PxBoxGeometry geometry = {};
                    m_shape->getBoxGeometry(geometry);
                    return kl::Float4x4::scaling(reinterpret_cast<kl::Float3&>(geometry.halfExtents));
                }
                case physx::PxGeometryType::Enum::eSPHERE: {
                    physx::PxSphereGeometry geometry = {};
                    m_shape->getSphereGeometry(geometry);
                    return kl::Float4x4::scaling(kl::Float3{ geometry.radius });
                }
                case physx::PxGeometryType::Enum::eCAPSULE: {
                    physx::PxCapsuleGeometry geometry = {};
                    m_shape->getCapsuleGeometry(geometry);
                    return kl::Float4x4::scaling(kl::Float3{ geometry.halfHeight * 0.5f, kl::Float2{geometry.radius} });
                }
                case physx::PxGeometryType::Enum::eTRIANGLEMESH: {
                    physx::PxTriangleMeshGeometry geometry = {};
                    m_shape->getTriangleMeshGeometry(geometry);
                    return kl::Float4x4::scaling(reinterpret_cast<kl::Float3&>(geometry.scale.scale));
                }
            }
            return {};
        }

        // Geometry
        void set_geometry(const physx::PxGeometry& geometry)
        {
            if (m_shape) {
                m_shape->release();
                m_shape = nullptr;
            }
            m_shape = m_physics->createShape(geometry, *m_material);
            kl::assert(m_shape, "Failed to create collider SHAPE");
        }

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
        physx::PxPhysics* m_physics = nullptr;
        physx::PxMaterial* m_material = nullptr;
        physx::PxShape* m_shape = nullptr;
    };
}
