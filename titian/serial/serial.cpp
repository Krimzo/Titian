#include "titian.h"


titian::String titian::format_serial_version( int32_t version )
{
    String result = kl::format( std::hex, std::setfill( '0' ), std::setw( 8 ), version );
    result.insert( result.begin() + 2, '\'' );
    result.insert( result.begin() + 5, '\'' );
    return result;
}
