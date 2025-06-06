#include "titian.h"


titian::TextSerializer::TextSerializer( StringRef const& path, bool write )
    : m_path( path ), m_writing( write )
{
    if ( write )
    {
        current()["serial_version"] = js::make_string( SERIAL_VERSION_STR );
        m_is_valid = true;
        Logger::log( "Opened TEXT serialization file [", path, "] (", SERIAL_VERSION_STR, ")" );
    }
    else
    {
        current() = { kl::read_file( path ) };

        String version;
        if ( current().contains( "serial_version" ) )
            version = current().at( "serial_version" )->get_string().value_or( version );

        if ( version == SERIAL_VERSION_STR )
        {
            m_is_valid = true;
            Logger::log( "Opened TEXT deserialization file [", path, "] (", SERIAL_VERSION_STR, ")" );
        }
        else
        {
            Logger::log( "Failed to verify [", path, "] serial version (", version, "), expected version (", SERIAL_VERSION_STR, ")" );
        }
    }
}

titian::TextSerializer::~TextSerializer()
{
    kl::assert( m_stack.size() == 1, "TextSerializer stack size should be 1" );
    if ( !*this )
        return;

    if ( m_writing )
        kl::write_file( m_path, current().decompile() );

    Logger::log( "Closed TEXT ", m_writing ? "serialization" : "deserialization", " file [", m_path, "]" );
}

titian::TextSerializer::operator bool() const
{
    return m_is_valid;
}

void titian::TextSerializer::push_object( StringRef const& name )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    auto& [_, val] = *current().emplace( name, new js::Object() ).first;
    m_stack.push_back( val.as<js::Object>() );
}

void titian::TextSerializer::pop_object()
{
    if ( m_stack.size() > 1 )
        m_stack.pop_back();
}

void titian::TextSerializer::load_object( StringRef const& name ) const
{
    auto& [_, val] = *current().find( name );
    m_stack.push_back( val.as<js::Object>() );
}

void titian::TextSerializer::unload_object() const
{
    if ( m_stack.size() > 1 )
        m_stack.pop_back();
}

void titian::TextSerializer::write_bool( StringRef const& name, bool value )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    current().emplace( name, js::make_bool( value ) );
}

void titian::TextSerializer::read_bool( StringRef const& name, bool& value ) const
{
    auto it = current().find( name );
    if ( it != current().end() )
        value = it->second->get_bool().value_or( value );
}

void titian::TextSerializer::write_int( StringRef const& name, int32_t value )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    current().emplace( name, js::make_number( value ) );
}

void titian::TextSerializer::read_int( StringRef const& name, int32_t& value ) const
{
    auto it = current().find( name );
    if ( it != current().end() )
        value = it->second->get_int().value_or( value );
}

void titian::TextSerializer::write_float( StringRef const& name, float value )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    current().emplace( name, js::make_number( value ) );
}

void titian::TextSerializer::read_float( StringRef const& name, float& value ) const
{
    auto it = current().find( name );
    if ( it != current().end() )
        value = it->second->get_float().value_or( value );
}

void titian::TextSerializer::write_byte_array( StringRef const& name, void const* data, int32_t count )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    byte* ptr = (byte*) data;
    Ref result = new js::Array();
    for ( int32_t i = 0; i < count; i++ )
        result->push_back( js::make_number( ptr[i] ) );
    current().emplace( name, std::move( result ) );
}

void titian::TextSerializer::read_byte_array( StringRef const& name, void* data, int32_t count ) const
{
    auto it = current().find( name );
    if ( it == current().end() )
        return;

    byte* ptr = (byte*) data;
    if ( auto container = it->second.as<js::Array>() )
    {
        int32_t min_count = kl::min( (int32_t) container->size(), count );
        for ( int32_t i = 0; i < min_count; i++ )
            ptr[i] = container->at( i )->get_int().value_or( 0 );
    }
}

void titian::TextSerializer::write_int_array( StringRef const& name, int32_t const* data, int32_t count )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    Ref result = new js::Array();
    for ( int32_t i = 0; i < count; i++ )
        result->push_back( js::make_number( data[i] ) );
    current().emplace( name, std::move( result ) );
}

void titian::TextSerializer::read_int_array( StringRef const& name, int32_t* data, int32_t count ) const
{
    auto it = current().find( name );
    if ( it == current().end() )
        return;

    if ( auto container = it->second.as<js::Array>() )
    {
        int32_t min_count = kl::min( (int32_t) container->size(), count );
        for ( int32_t i = 0; i < min_count; i++ )
            data[i] = container->at( i )->get_int().value_or( 0 );
    }
}

void titian::TextSerializer::write_float_array( StringRef const& name, float const* data, int32_t count )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    Ref result = new js::Array();
    for ( int32_t i = 0; i < count; i++ )
        result->push_back( js::make_number( data[i] ) );
    current().emplace( name, std::move( result ) );
}

void titian::TextSerializer::read_float_array( StringRef const& name, float* data, int32_t count ) const
{
    auto it = current().find( name );
    if ( it == current().end() )
        return;

    if ( auto container = it->second.as<js::Array>() )
    {
        int32_t min_count = kl::min( (int32_t) container->size(), count );
        for ( int32_t i = 0; i < min_count; i++ )
            data[i] = container->at( i )->get_float().value_or( 0.0f );
    }
}

void titian::TextSerializer::write_string( StringRef const& name, StringRef const& value )
{
    kl::assert( !current().contains( name ), "TextSerialzer object already contains key ", name );
    current().emplace( name, js::make_string( value ) );
}

void titian::TextSerializer::read_string( StringRef const& name, String& value ) const
{
    auto it = current().find( name );
    if ( it != current().end() )
        value = it->second->get_string().value_or( value );
}
