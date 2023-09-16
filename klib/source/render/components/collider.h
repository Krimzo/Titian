#pragma once

#include "math/math.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class collider
    {
        PxMaterial* material_ = nullptr;
        PxShape* shape_ = nullptr;

    public:
        collider(PxPhysics* physics, const PxGeometry& geometry);
        virtual ~collider();

        collider(const collider&) = delete;
        collider(const collider&&) = delete;

        void operator=(const collider&) = delete;
        void operator=(const collider&&) = delete;

        // Get
        PxShape* shape();
        PxGeometryType::Enum type() const;
        float4x4 scaling_matrix() const;

        // Geometry
        void set_rotation(const float3& rotation);
        float3 rotation() const;

        void set_offset(const float3& position);
        float3 offset() const;

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
