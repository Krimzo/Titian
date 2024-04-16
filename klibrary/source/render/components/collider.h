#pragma once

#include "math/math.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class Collider
    {
        physx::PxMaterial* m_material = nullptr;
        physx::PxShape* m_shape = nullptr;

    public:
        Collider(physx::PxPhysics* physics, const physx::PxGeometry& geometry);
        virtual ~Collider();

        Collider(const Collider&) = delete;
        Collider(Collider&&) = delete;

        void operator=(const Collider&) = delete;
        void operator=(Collider&&) = delete;

        // Get
        physx::PxShape* shape() const;
        physx::PxGeometryType::Enum type() const;
        Float4x4 scaling_matrix() const;

        // Geometry
        void set_rotation(const Float3& rotation);
        Float3 rotation() const;

        void set_offset(const Float3& position);
        Float3 offset() const;

        // Material
        void set_static_friction(float friction);
        float static_friction() const;

        void set_dynamic_friction(float friction);
        float dynamic_friction() const;

        void set_restitution(float restitution);
        float restitution() const;
    };
}

#endif
