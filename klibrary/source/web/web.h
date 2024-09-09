#pragma once

#include "web/socket/socket.h"


namespace kl {
    std::string download_website(const std::string_view& url, int buffer_size = 65536);
}
