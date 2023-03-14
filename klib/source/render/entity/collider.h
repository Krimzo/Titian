#pragma once

#ifdef KL_USING_PHYSX

#include <PxPhysicsAPI.h>

using namespace physx;


namespace kl {
    class collider
    {
        PxPhysics* physics_ = nullptr;
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
        PxGeometryType::Enum get_type() const;
        PxShape* get_shape();

        // Material
        float get_static_friction() const;
        void set_static_friction(float friction);

        float get_dynamic_friction() const;
        void set_dynamic_friction(float friction);

        float get_restitution() const;
        void set_restitution(float restitution);
    
    };
}

#endif
