#pragma once

#include "web/socket/socket.h"


namespace kl
{
std::string download_website( std::string_view const& url, int buffer_size = 65536 );
}
