#pragma once

#include "web/http/http_request.h"
#include "web/http/http_response.h"


namespace kl
{
struct HttpServer
{
    using RequestHandler = std::function<HttpResponse( HttpRequest const& )>;

    RequestHandler request_handler = []( auto _ ) { return HttpResponse{}; };

    HttpServer() = default;

    void start( int port, int max_clients = 100 );
    void close();

private:
    kl::TCPSocket m_socket;
    std::vector<std::jthread> m_workers;

    void handle_server();
    bool handle_client( kl::TCPSocket& client ) const;
};
}
