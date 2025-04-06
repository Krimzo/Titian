#pragma once

#include "math/math.h"


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
struct Socket : NoCopy
{
    using ID = uint64_t;
    static std::string SELF;

    Socket( bool udp );
    ~Socket();

    ID id() const;
    operator bool() const;

    std::string address() const;
    int set_address( std::string_view const& address );

    int port() const;
    void set_port( int port );

    int bind();
    int listen( int queue_size );
    void accept( Socket& socket );
    int connect();

    int send( void const* data, int byte_size ) const;
    int receive( void* buff, int byte_size ) const;

    template <typename T>
    bool send( T const& obj ) const
    {
        return send( &obj, sizeof( T ) ) == sizeof( T );
    }

    template <typename T>
    bool receive( T* obj ) const
    {
        return receive( obj, sizeof( T ) ) == sizeof( T );
    }

    template <typename T>
    T receive() const
    {
        T t{};
        receive( &t );
        return t;
    }

    int send_to( void const* data, int byte_size, Address const& address ) const;
    int receive_from( void* buff, int byte_size, Address* address ) const;

    template <typename T>
    bool send_to( T const& obj, Address const& address ) const
    {
        return send_to( &obj, sizeof( T ), address ) == sizeof( T );
    }

    template <typename T>
    bool receive_from( T* obj, Address* address ) const
    {
        return receive_from( obj, sizeof( T ), address ) == sizeof( T );
    }

    template <typename T>
    T receive_from( Address* address ) const
    {
        T t{};
        receive_from( &t, address );
        return t;
    }

    int exhaust( std::vector<byte>& output, int buffer_size = 16384 ) const;

private:
    Address m_address = {};
    ID m_socket = {};
};
}
