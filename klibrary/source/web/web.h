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
std::string download_website( std::string_view const& url, int buffer_size = 65536 );
}
