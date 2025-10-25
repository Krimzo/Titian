#pragma once

#include "web/http/http_request.h"


namespace kl
{
struct HttpQuery : std::map<std::string, std::string>
{
    std::string path;

    HttpQuery() = default;
    HttpQuery( std::string_view const& full_path );
};

std::string encode_url_string( std::string_view const& data );
std::string decode_url_string( std::string_view const& data );
}
