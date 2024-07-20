#pragma once

#include "components/mesh.h"


namespace titian {
    class Collider : public Serializable
    {
    public:
        Collider(px::PxPhysics* physics);
        Collider(px::PxPhysics* physics, const px::PxGeometry& geometry);
        ~Collider() override;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        // Get
        px::PxShape* shape() const;
        px::PxGeometryType::Enum type() const;
        kl::Float4x4 scaling_matrix() const;

        // Geometry
        void set_geometry(const px::PxGeometry& geometry);

        void set_rotation(const kl::Float3& rotation);
        kl::Float3 rotation() const;

        void set_offset(const kl::Float3& position);
        kl::Float3 offset() const;

        // Material
        void set_static_friction(float friction);
        float static_friction() const;

        void set_dynamic_friction(float friction);
        float dynamic_friction() const;

        void set_restitution(float restitution);
        float restitution() const;

    private:
        px::PxPhysics* m_physics = nullptr;
        px::PxMaterial* m_material = nullptr;
        px::PxShape* m_shape = nullptr;
    };
}
