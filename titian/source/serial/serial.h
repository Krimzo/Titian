#pragma once

#include "app/logger.h"


namespace titian
{
String format_serial_version( int32_t version );
inline constexpr int32_t SERIAL_VERSION_INT = 0x23'11'2024;
inline String SERIAL_VERSION_STR = format_serial_version( SERIAL_VERSION_INT );
}

namespace titian
{
struct Serializer : kl::NoCopy
{
    Serializer() = default;
    virtual ~Serializer() = default;

    virtual operator bool() const = 0;

    virtual void push_object( StringRef const& name ) = 0;
    virtual void pop_object() = 0;

    virtual void load_object( StringRef const& name ) const = 0;
    virtual void unload_object() const = 0;

    virtual void write_bool( StringRef const& name, bool value ) = 0;
    virtual void read_bool( StringRef const& name, bool& value ) const = 0;

    virtual void write_int( StringRef const& name, int32_t value ) = 0;
    virtual void read_int( StringRef const& name, int32_t& value ) const = 0;

    virtual void write_float( StringRef const& name, float value ) = 0;
    virtual void read_float( StringRef const& name, float& value ) const = 0;

    virtual void write_byte_array( StringRef const& name, void const* data, int32_t count ) = 0;
    virtual void read_byte_array( StringRef const& name, void* data, int32_t count ) const = 0;

    virtual void write_int_array( StringRef const& name, int32_t const* data, int32_t count ) = 0;
    virtual void read_int_array( StringRef const& name, int32_t* data, int32_t count ) const = 0;

    virtual void write_float_array( StringRef const& name, float const* data, int32_t count ) = 0;
    virtual void read_float_array( StringRef const& name, float* data, int32_t count ) const = 0;

    virtual void write_string( StringRef const& name, StringRef const& value ) = 0;
    virtual void read_string( StringRef const& name, String& value ) const = 0;
};
}

namespace titian
{
struct Serializable
{
    Serializable() = default;
    virtual ~Serializable() = default;

    virtual void serialize( Serializer& serializer ) const = 0;
    virtual void deserialize( Serializer const& serializer ) = 0;
};
}
