#include "klibrary.h"


kl::HttpApp::HttpApp()
{
    using namespace std::placeholders;
    HttpServer::request_handler = std::bind( &HttpApp::handle_request, this, _1 );
}

void kl::HttpApp::start( int port )
{
    HttpServer::start( port );
}

kl::HttpResponse kl::HttpApp::handle_request( HttpRequest const& request ) const
{
    const HttpQuery query{ request.full_path };
    HttpResponse response;
    if ( auto* opt_route = find_exact_route( query.path ) )
    {
        ( *opt_route )( request, query, response );
    }
    else if ( auto opt_route = find_starting_route( query.path ) )
    {
        opt_route->second( request, query, query.path.substr( opt_route->first.size() ), response );
    }
    else
    {
        default_route( request, query, response );
    }
    return response;
}

kl::HttpApp::ExactRoute const* kl::HttpApp::find_exact_route( std::string const& path ) const
{
    const auto it = exact_routes.find( path );
    if ( it == exact_routes.end() )
        return nullptr;
    return &it->second;
}

std::optional<std::pair<std::string_view, kl::HttpApp::StartingRoute>> kl::HttpApp::find_starting_route( std::string_view const& path ) const
{
    for ( auto const& [key, value] : starting_routes )
    {
        if ( path.starts_with( key ) )
            return std::pair<std::string_view, kl::HttpApp::StartingRoute>{ key, value };
    }
    return std::nullopt;
}
