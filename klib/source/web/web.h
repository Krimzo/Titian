#pragma once

#include <string>


namespace kl::web {
    std::string download_website(const std::string& url, int buffer_size = 65536);
}
