#pragma once

#include "utility/hash/hash_t.h"


namespace kl
{
Hash hash( void const* data, uint64_t byte_size );
Hash hash_str( std::string_view const& data );

template<typename T>
Hash hash_obj( T const& object )
{
    return hash( &object, sizeof( T ) );
}
}
