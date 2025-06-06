#include "titian.h"


void titian::NativeScript::serialize( Serializer& serializer ) const
{
    Script::serialize( serializer );

    int32_t data_size = (int32_t) data.size();
    serializer.write_int( "data_size", data_size );
    serializer.write_byte_array( "data", data.data(), data_size );
}

void titian::NativeScript::deserialize( Serializer const& serializer )
{
    Script::deserialize( serializer );

    int32_t data_size = 0;
    serializer.read_int( "data_size", data_size );
    data.resize( data_size );
    serializer.read_byte_array( "data", data.data(), data_size );

    reload();
}

bool titian::NativeScript::is_valid() const
{
    return (bool) m_memory_module;
}

void titian::NativeScript::reload()
{
    unload();

    m_memory_module = MemoryLoadLibrary( data.data(), data.size() );
    m_start_function = read_function<void, Scene&>( "on_start" );
    m_update_function = read_function<void, Scene&>( "on_update" );
    m_collision_function = read_function<void, Scene&, Entity&, Entity&>( "on_collision" );
    m_ui_function = read_function<void, Scene&>( "on_ui" );
}

void titian::NativeScript::call_start( Scene& scene )
{
    if ( m_start_function )
        m_start_function( scene );
}

void titian::NativeScript::call_update( Scene& scene )
{
    if ( m_update_function )
        m_update_function( scene );
}

void titian::NativeScript::call_collision( Scene& scene, Entity& attacker, Entity& target )
{
    if ( m_collision_function )
        m_collision_function( scene, attacker, target );
}

void titian::NativeScript::call_ui( Scene& scene )
{
    if ( m_ui_function )
        m_ui_function( scene );
}

void titian::NativeScript::unload()
{
    m_ui_function = nullptr;
    m_collision_function = nullptr;
    m_update_function = nullptr;
    m_start_function = nullptr;

    if ( !m_memory_module )
        return;

    MemoryFreeLibrary( m_memory_module );
    m_memory_module = nullptr;
}
