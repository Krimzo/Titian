#pragma once

#include "scene/entity.h"


namespace titian
{
struct AmbientLight : Entity
{
    Float3 color{ 0.1f };

    AmbientLight();

    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;
};
}
