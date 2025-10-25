#pragma once

#include "web/http/http_server.h"


namespace kl
{
struct HttpApp : private HttpServer
{
    using ExactRoute = std::function<void( HttpRequest const&, HttpQuery const&, HttpResponse& )>;
    using StartingRoute = std::function<void( HttpRequest const&, HttpQuery const&, std::string_view const&, HttpResponse& )>;
    using DefaultRoute = std::function<void( HttpRequest const&, HttpQuery const&, HttpResponse& )>;

    std::map<std::string, ExactRoute> exact_routes;
    std::map<std::string, StartingRoute> starting_routes;
    DefaultRoute default_route = []( HttpRequest const& request, HttpQuery const& query, HttpResponse& response )
        {
            response.status_code = HttpStatusCode::NOT_FOUND;
            response.load_text( "Does not exist." );
        };

    HttpApp();

    void start( int port );

private:
    HttpResponse handle_request( HttpRequest const& request ) const;

    ExactRoute const* find_exact_route( std::string const& path ) const;
    std::optional<std::pair<std::string_view, StartingRoute>> find_starting_route( std::string_view const& path ) const;
};
}
