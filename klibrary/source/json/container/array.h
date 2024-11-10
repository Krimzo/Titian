#pragma once

#include "json/container/container.h"


namespace kl::json
{
struct Array : ArrayStorage, Container
{
    Array();
    Array( std::string_view const& data );

    bool compile( std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last ) override;
    std::string decompile( int depth = 0 ) const override;
};
}
