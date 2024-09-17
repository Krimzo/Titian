#pragma once

#include "components/mesh.h"


namespace titian {
    struct Collider : Serializable
    {
        Collider();
        ~Collider() override;

        void serialize(Serializer* serializer) const override;
        void deserialize(const Serializer* serializer) override;

        px::PxGeometryType::Enum type() const;
        px::PxShape* shape() const;

        void set_geometry(const px::PxGeometry& geometry);
        px::PxGeometryHolder geometry() const;

        void set_rotation(const Float3& rotation);
        Float3 rotation() const;

        void set_offset(const Float3& position);
        Float3 offset() const;

        void set_static_friction(float friction);
        float static_friction() const;

        void set_dynamic_friction(float friction);
        float dynamic_friction() const;

        void set_restitution(float restitution);
        float restitution() const;

        Float4x4 scaling_matrix() const;

        Ref<Collider> clone() const;

    private:
        px::PxMaterial* const m_material;
        px::PxShape* m_shape = nullptr;
    };
}
