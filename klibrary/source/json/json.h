#pragma once

#include "json/language/standard.h"
#include "json/language/lexer.h"
#include "json/container/container.h"
#include "json/container/literal.h"
#include "json/container/object.h"
#include "json/container/array.h"


namespace kl::json
{
struct ContainerSerializable
{
    virtual ~ContainerSerializable() = default;
    virtual Ref<Container> to_container() = 0;
    virtual void from_container( Container const& container ) = 0;
};

struct ObjectSerializable : ContainerSerializable
{
    virtual void to_object( Object& object ) = 0;
    virtual void from_object( Object const& object ) = 0;

    Ref<Container> to_container() final
    {
        Ref container = new Object();
        this->to_object( *container );
        return container;
    }

    void from_container( Container const& container ) final
    {
        if ( Object const* object = dynamic_cast<Object const*>(&container) )
            this->from_object( *object );
    }
};

struct ArraySerializable : ContainerSerializable
{
    virtual void to_array( Array& array ) = 0;
    virtual void from_array( Array const& array ) = 0;

    Ref<Container> to_container() final
    {
        Ref container = new Array();
        this->to_array( *container );
        return container;
    }

    void from_container( Container const& container ) final
    {
        if ( Array const* array = dynamic_cast<Array const*>(&container) )
            this->from_array( *array );
    }
};
}
