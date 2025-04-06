#include "klibrary.h"


static int _socket_init = []
{
    WSADATA wsa_data{};
    int result = ::WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
    kl::assert( result == 0, "Failed to initialize WSA" );
    return result;
}();

kl::Address::Address()
    : sockaddr_in()
{
    sin_family = AF_INET;
}

std::string kl::Address::address() const
{
    char buffer[INET_ADDRSTRLEN + 1] = {};
    return ::inet_ntop( AF_INET, &sin_addr, buffer, INET_ADDRSTRLEN );
}

int kl::Address::set_address( std::string_view const& address )
{
    return ::inet_pton( AF_INET, address.data(), &sin_addr );
}

int kl::Address::port() const
{
    return (int) ::ntohs( sin_port );
}

void kl::Address::set_port( int port )
{
    sin_port = ::htons( (u_short) port );
}

std::string kl::Socket::SELF = "127.0.0.1";

kl::Socket::Socket( bool udp )
{
    m_socket = ::socket( AF_INET,
        udp ? SOCK_DGRAM : SOCK_STREAM,
        udp ? IPPROTO_UDP : IPPROTO_TCP );
    m_address.sin_addr.s_addr = INADDR_ANY;
    verify( m_socket != INVALID_SOCKET, "Failed to create socket" );
}

kl::Socket::~Socket()
{
    ::closesocket( m_socket );
}

kl::Socket::ID kl::Socket::id() const
{
    return m_socket;
}

kl::Socket::operator bool() const
{
    return m_socket != INVALID_SOCKET;
}

std::string kl::Socket::address() const
{
    return m_address.address();
}

int kl::Socket::set_address( std::string_view const& address )
{
    return m_address.set_address( address );
}

int kl::Socket::port() const
{
    return m_address.port();
}

void kl::Socket::set_port( int port )
{
    m_address.set_port( port );
}

int kl::Socket::bind()
{
    return ::bind( m_socket, (sockaddr*) &m_address, sizeof( Address ) );
}

int kl::Socket::listen( int queue_size )
{
    return ::listen( m_socket, queue_size );
}

void kl::Socket::accept( Socket& socket )
{
    int address_length = sizeof( Address );
    socket.m_socket = ::accept( m_socket, (sockaddr*) &m_address, &address_length );
}

int kl::Socket::connect()
{
    return ::connect( m_socket, (sockaddr*) &m_address, sizeof( Address ) );
}

int kl::Socket::send( void const* data, int byte_size ) const
{
    return ::send( m_socket, (char const*) data, byte_size, NULL );
}

int kl::Socket::receive( void* buff, int byte_size ) const
{
    return ::recv( m_socket, (char*) buff, byte_size, NULL );
}

int kl::Socket::send_to( void const* data, int byte_size, Address const& address ) const
{
    return ::sendto( m_socket, (char const*) data, byte_size, NULL, (sockaddr const*) &address, sizeof( Address ) );
}

int kl::Socket::receive_from( void* buff, int byte_size, Address* address ) const
{
    int address_length = sizeof( Address );
    return ::recvfrom( m_socket, (char*) buff, byte_size, NULL, (sockaddr*) address, &address_length );
}

int kl::Socket::exhaust( std::vector<byte>& output, int buffer_size ) const
{
    std::vector<byte> receiver_buffer( buffer_size );
    int total_received = 0;
    for ( int received; (received = receive( receiver_buffer.data(), buffer_size )) > 0;)
    {
        output.insert( output.end(), receiver_buffer.begin(), receiver_buffer.begin() + received );
        total_received += received;
    }
    return total_received;
}
