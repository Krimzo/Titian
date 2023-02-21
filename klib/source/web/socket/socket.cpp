#include "web/socket/socket.h"

#include "utility/utility.h"


// Init
namespace kl_ignored {
    static const int dont_care = []()
    {
        WSADATA wsa_data = {};
        kl::error_check(WSAStartup(MAKEWORD(2, 2), &wsa_data), "Failed to initialize WSA");
        return 0;
    }();
}

// Construct
kl::socket::socket()
{
    socket_ = ::socket(AF_INET, SOCK_STREAM, NULL);
    error_check(socket_ == INVALID_SOCKET, "Failed to create socket");
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
}

kl::socket::socket(const std::string& address, const int port)
    : socket()
{
    set_address(address);
    set_port(port);
}

kl::socket::socket(const size_t socket)
    : socket_(socket)
{}

kl::socket::~socket()
{
    close();
}

// Methods
size_t kl::socket::get_id() const
{
    return socket_;
}

void kl::socket::close() const
{
    closesocket(socket_);
}

std::string kl::socket::get_address() const
{
    char buffer[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &address_.sin_addr, buffer, INET_ADDRSTRLEN);
    return buffer;
}

void kl::socket::set_address(const std::string& address)
{
    error_check(inet_pton(AF_INET, address.c_str(), &address_.sin_addr) != 1, "Could not convert address");
}

int kl::socket::get_port() const
{
    return ntohs(address_.sin_port);
}

void kl::socket::set_port(const int port)
{
    address_.sin_port = htons(u_short(port));
}

void kl::socket::listen(const int queue_size)
{
    error_check(bind(socket_, (sockaddr*) &address_, sizeof(address_)), "Could not bind socket");
    error_check(::listen(socket_, queue_size), "Could not listen on socket");
}

kl::socket kl::socket::accept()
{
    int address_length = sizeof(address_);
    const size_t accepted = ::accept(socket_, (sockaddr*) &address_, &address_length);
    error_check(accepted == INVALID_SOCKET, "Could not accept socket");
    return socket(accepted);
}

void kl::socket::connect()
{
    error_check(::connect(socket_, (sockaddr*) &address_, sizeof(address_)), "Could not connect to socket");
}

int kl::socket::send(const void* data, const int byte_size) const
{
    return ::send(socket_, (const char*) data, byte_size, NULL);
}

int kl::socket::receive(void* buff, const int byte_size) const
{
    return recv(socket_, (char*) buff, byte_size, NULL);
}

// Static
const std::string kl::socket::self_ip = "127.0.0.1";
