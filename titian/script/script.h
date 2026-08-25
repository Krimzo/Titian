#pragma once

#include "scene/entity.h"


namespace titian
{
struct Scene;
}

namespace titian
{
struct Script : kl::NoCopy, Serializable
{
    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;

    virtual bool is_valid() const = 0;
    virtual void reload() = 0;

    virtual void call_start( Scene& scene ) = 0;
    virtual void call_update( Scene& scene ) = 0;
    virtual void call_collision( Scene& scene, Entity& attacker, Entity& target ) = 0;
    virtual void call_ui( Scene& scene ) = 0;
};
}
