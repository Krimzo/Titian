#pragma once

#include "web/html/html.h"
#include "web/http/http_app.h"
#include "web/http/http_server.h"
#include "web/http/http_response.h"
#include "web/http/http_query.h"
#include "web/http/http_request.h"
#include "web/socket/socket.h"


namespace kl
{
struct Internet : NoCopy, NoMove
{
    HINTERNET ptr;

    Internet( HINTERNET ptr )
        : ptr( ptr )
    {}

    ~Internet() noexcept
    {
        InternetCloseHandle( ptr );
    }

    operator HINTERNET() const noexcept
    {
        return ptr;
    }
};
}

namespace kl
{
std::string download_website( std::string_view const& url, int buffer_size = 65536 );
int send_http_request( std::string_view const& url, int port, std::string_view const& object, std::string_view const& verb, char const* headers, DWORD headers_size, void const* optional, DWORD optional_size );
}
