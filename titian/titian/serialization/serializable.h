#pragma once

#include "serialization/serializer.h"


namespace titian {
    class Serializable
    {
    public:
        Serializable() = default;
        virtual ~Serializable() = default;
        
        virtual void serialize(Serializer* serializer, const void* helper_data) const = 0;
        virtual void deserialize(const Serializer * serializer, const void* helper_data) = 0;
    };
}
