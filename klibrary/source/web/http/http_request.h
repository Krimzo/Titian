#pragma once

#include "memory/files/file.h"
#include "web/socket/socket.h"


namespace kl
{
struct HttpRequestType
{
    static constexpr std::string_view GET = "GET";
    static constexpr std::string_view POST = "POST";
};

struct HttpRequest
{
    std::string request_type;
    std::string full_path;
    std::string http_version;
    std::map<std::string, std::string> values;

    HttpRequest() = default;

    std::optional<ByteRange> byte_range() const;

    static std::optional<HttpRequest> from_string( std::string_view const& data );
};
}
