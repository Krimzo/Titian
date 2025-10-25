#pragma once

#include "math/math.h"


namespace kl
{
inline constexpr std::string_view SELF_ADDRESS = "127.0.0.1";
}

namespace kl
{
struct Address : sockaddr_in
{
    Address();

    std::string address() const;
    int set_address( std::string_view const& address );

    int port() const;
    void set_port( int port );
};
}

namespace kl
{
struct TCPSocket : NoCopy
{
    Address address;
    SOCKET socket = INVALID_SOCKET;

    TCPSocket();
    ~TCPSocket() noexcept;

    operator bool() const;
    void open();
    void close();

    int server_bind();
    int server_listen( int queue_size );
    void server_accept( TCPSocket& out_client );
    int client_connect();

    void set_blocking( bool enabled );
    int available() const;

    int send( void const* data, int byte_size ) const;
    int receive( void* buff, int byte_size ) const;
    int64_t receive_all( std::vector<byte>& output, int buffer_size = 16384 ) const;

    template <typename T>
    bool send( T const& obj ) const
    {
        return send( &obj, sizeof( T ) ) == sizeof( T );
    }

    template <typename T>
    bool receive( T& out_obj ) const
    {
        return receive( &out_obj, sizeof( T ) ) == sizeof( T );
    }

    template <typename T>
    std::optional<T> receive() const
    {
        T obj{};
        if ( receive<T>( obj ) )
            return obj;
        else
            return std::nullopt;
    }
};
}

namespace kl
{
struct UDPSocket : NoCopy
{
    Address address;
    SOCKET socket = INVALID_SOCKET;

    UDPSocket();
    ~UDPSocket() noexcept;

    operator bool() const;
    void open();
    void close();

    int server_bind();

    void set_blocking( bool enabled );
    int available() const;

    int send( void const* data, int byte_size, Address const& address ) const;
    int receive( void* buff, int byte_size, Address& out_address ) const;

    template <typename T>
    bool send( T const& obj, Address const& address ) const
    {
        return send( &obj, sizeof( T ), address ) == sizeof( T );
    }

    template <typename T>
    bool receive( T& out_obj, Address& out_address ) const
    {
        return receive( &out_obj, sizeof( T ), out_address ) == sizeof( T );
    }

    template <typename T>
    std::optional<T> receive( Address& out_address ) const
    {
        T obj{};
        if ( receive<T>( obj, out_address ) )
            return obj;
        else
            return std::nullopt;
    }
};
}

namespace kl
{
struct SocketInit
{
    static const int _init;
};
}
