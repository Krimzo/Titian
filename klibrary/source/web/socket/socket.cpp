#include "klibrary.h"


const int kl::SocketInit::_init = []() -> int
    {
        WSADATA wsa_data{};
        kl::assert( ::WSAStartup( MAKEWORD( 2, 2 ), &wsa_data ) == 0, "Failed to initialize WSA" );
        return {};
    }( );

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
    return ( int ) ::ntohs( sin_port );
}

void kl::Address::set_port( int port )
{
    sin_port = ::htons( (u_short) port );
}

kl::TCPSocket::TCPSocket()
{
    open();
}

kl::TCPSocket::~TCPSocket() noexcept
{
    close();
}

kl::TCPSocket::operator bool() const
{
    return socket != INVALID_SOCKET;
}

void kl::TCPSocket::open()
{
    address.sin_addr.s_addr = INADDR_ANY;
    socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
}

void kl::TCPSocket::close()
{
    ::closesocket( socket );
    socket = INVALID_SOCKET;
}

int kl::TCPSocket::server_bind()
{
    return ::bind( socket, (sockaddr*) &address, sizeof( Address ) );
}

int kl::TCPSocket::server_listen( int queue_size )
{
    return ::listen( socket, queue_size );
}

void kl::TCPSocket::server_accept( TCPSocket& out_client )
{
    int _ = sizeof( Address );
    out_client.socket = ::accept( socket, (sockaddr*) &address, &_ );
}

int kl::TCPSocket::client_connect()
{
    return ::connect( socket, (sockaddr*) &address, sizeof( Address ) );
}

void kl::TCPSocket::set_blocking( bool enabled )
{
    u_long mode = enabled ? 0 : 1;
    ::ioctlsocket( socket, FIONBIO, &mode );
}

int kl::TCPSocket::available() const
{
    u_long available = 0;
    ::ioctlsocket( socket, FIONREAD, &available );
    return (int) available;
}

int kl::TCPSocket::send( void const* data, int byte_size ) const
{
    return ::send( socket, (char const*) data, byte_size, NULL );
}

int kl::TCPSocket::receive( void* buff, int byte_size ) const
{
    return ::recv( socket, (char*) buff, byte_size, NULL );
}

int64_t kl::TCPSocket::receive_all( std::vector<byte>& output, int buffer_size ) const
{
    int64_t total_received = 0;
    std::vector<byte> receiver_buffer( buffer_size );
    while ( true )
    {
        const int received = receive( receiver_buffer.data(), buffer_size );
        if ( received < 0 )
            return received;
        if ( received == 0 )
            break;
        output.insert( output.end(), receiver_buffer.begin(), receiver_buffer.begin() + received );
        total_received += received;
        if ( available() <= 0 )
            break;
    }
    return total_received;
}

kl::UDPSocket::UDPSocket()
{
    open();
}

kl::UDPSocket::~UDPSocket()
{
    close();
}

kl::UDPSocket::operator bool() const
{
    return socket != INVALID_SOCKET;
}

void kl::UDPSocket::open()
{
    address.sin_addr.s_addr = INADDR_ANY;
    socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
}

void kl::UDPSocket::close()
{
    ::closesocket( socket );
    socket = INVALID_SOCKET;
}

int kl::UDPSocket::server_bind()
{
    return ::bind( socket, (sockaddr*) &address, sizeof( Address ) );
}

void kl::UDPSocket::set_blocking( bool enabled )
{
    u_long mode = enabled ? 0 : 1;
    ::ioctlsocket( socket, FIONBIO, &mode );
}

int kl::UDPSocket::available() const
{
    u_long available = 0;
    ::ioctlsocket( socket, FIONREAD, &available );
    return (int) available;
}

int kl::UDPSocket::send( void const* data, int byte_size, Address const& address ) const
{
    return ::sendto( socket, (char const*) data, byte_size, NULL, (sockaddr const*) &address, sizeof( Address ) );
}

int kl::UDPSocket::receive( void* buff, int byte_size, Address& address ) const
{
    int address_length = sizeof( Address );
    return ::recvfrom( socket, (char*) buff, byte_size, NULL, (sockaddr*) &address, &address_length );
}
