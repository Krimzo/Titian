#pragma once

#include "json/language/standard.h"
#include "json/language/lexer.h"
#include "json/container/container.h"
#include "json/container/literal.h"
#include "json/container/object.h"
#include "json/container/array.h"


namespace kl::json {
    struct ContainerSerializable
    {
        virtual ~ContainerSerializable() = default;
        virtual Ref<Container> to_container() const = 0;
        virtual void from_container(const Container& container) = 0;
    };

    struct ObjectSerializable : ContainerSerializable
    {
        virtual void to_object(Object& object) const = 0;
        virtual void from_object(const Object& object) = 0;

        Ref<Container> to_container() const final
        {
            Ref container = new Object();
            this->to_object(*container);
            return container;
        }

        void from_container(const Container& container) final
        {
            if (const Object* object = dynamic_cast<const Object*>(&container)) {
                this->from_object(*object);
            }
        }
    };

    struct ArraySerializable : ContainerSerializable
    {
        virtual void to_array(Array& array) const = 0;
        virtual void from_array(const Array& array) = 0;

        Ref<Container> to_container() const final
        {
            Ref container = new Array();
            this->to_array(*container);
            return container;
        }

        void from_container(const Container& container) final
        {
            if (const Array* array = dynamic_cast<const Array*>(&container)) {
                this->from_array(*array);
            }
        }
    };
}
