#pragma once

#include "apis/apis.h"


namespace kl
{
struct Html
{
    std::string data;

    Html() = default;

    void apply_var( std::string_view const& var_name, std::string_view const& var_value );

    static Html from_string( std::string_view const& str );
    static Html from_file( std::string_view const& path );
};
}
