#pragma once

#include "ots/language/standard.h"
#include "ots/language/preprocessor.h"
#include "ots/language/parser.h"

#include "ots/container/data_container.h"
#include "ots/container/literal_container.h"
#include "ots/container/array_container.h"
#include "ots/container/map_container.h"


namespace kl::ots {
    class ContainerSerializable
    {
    public:
        virtual ~ContainerSerializable() = default;
        virtual Ref<DataContainer> to_container() const = 0;
        virtual void from_container(const DataContainer& container) = 0;
    };

    class ArraySerializable : public ContainerSerializable
    {
    public:
        virtual void to_array(ArrayContainer& array) const = 0;
        virtual void from_array(const ArrayContainer& array) = 0;

        Ref<DataContainer> to_container() const final
        {
            Ref container = new ArrayContainer();
            this->to_array(*container);
            return container;
        }

        void from_container(const DataContainer& container) final
        {
            if (const ArrayContainer* array_container = dynamic_cast<const ArrayContainer*>(&container)) {
                this->from_array(*array_container);
            }
        }
    };

    class MapSerializable : public ContainerSerializable
    {
    public:
        virtual void to_map(MapContainer& map) const = 0;
        virtual void from_map(const MapContainer& map) = 0;

        Ref<DataContainer> to_container() const final
        {
            Ref container = new MapContainer();
            this->to_map(*container);
            return container;
        }

        void from_container(const DataContainer& container) final
        {
            if (const MapContainer* map_container = dynamic_cast<const MapContainer*>(&container)) {
                this->from_map(*map_container);
            }
        }
    };
}
