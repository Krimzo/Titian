#include "titian.h"


titian::Layer::Layer( StringRef const& name )
    : BenchmarkInfo( name )
{
    static StringSet names;
    if ( names.contains( name ) )
        panic();
    names.emplace( name );
}

void titian::Layer::panic() const
{
    kl::assert( false, "Layer 0x", std::hex, size_t( this ), " panicked" );
}
