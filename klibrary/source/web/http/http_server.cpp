#include "klibrary.h"


void kl::HttpServer::start( int port, int max_clients )
{
    m_socket.address.set_port( port );
    m_socket.server_bind();
    m_socket.server_listen( max_clients );
    while ( m_socket )
        handle_server();
}

void kl::HttpServer::close()
{
    m_socket.close();
}

void kl::HttpServer::handle_server()
{
    Ref client = new TCPSocket{};
    m_socket.server_accept( *client );
    m_workers.emplace_back( [this, client] { while ( *client && handle_client( *client ) ); } );
}

bool kl::HttpServer::handle_client( TCPSocket& client ) const
{
    std::vector<byte> data;
    if ( client.receive_all( data ) <= 0 )
        return false;
    data.push_back( 0 );

    if ( auto opt_req = HttpRequest::from_string( (char const*) data.data() ) )
    {
        const HttpResponse response = request_handler( *opt_req );
        const std::string response_data = response.compile();
        client.send( response_data.data(), (int) response_data.size() );
        return true;
    }
    else
        return false;
}
