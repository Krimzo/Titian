#include "klib.h"


// Init
namespace kl_ignored {
    static const int dont_care = []()
    {
        WSADATA wsa_data = {};
        int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        kl::error_check(result, "Failed to initialize WSA");
        return result;
    }();
}

// Static
const std::string kl::socket::self = "127.0.0.1";

// Construct
kl::socket::socket()
{
    m_socket = ::socket(AF_INET, SOCK_STREAM, NULL);
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    warning_check(m_socket == INVALID_SOCKET, "Failed to create socket");
}

kl::socket::socket(const int port)
    : socket(self, port)
{}

kl::socket::socket(const std::string& address, const int port)
    : socket()
{
    set_address(address);
    set_port(port);
}

kl::socket::~socket()
{
    close();
}

// Properties
SOCKET kl::socket::id() const
{
    return m_socket;
}

std::string kl::socket::address() const
{
    char buffer[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &m_address.sin_addr, buffer, INET_ADDRSTRLEN);
    return { buffer };
}

int kl::socket::set_address(const std::string& address)
{
    int result = inet_pton(AF_INET, address.c_str(), &m_address.sin_addr);
    warning_check(result != 1, format("Could not convert address ", address));
    return result;
}

int kl::socket::port() const
{
    return (int) ntohs(m_address.sin_port);
}

void kl::socket::set_port(const int port)
{
    m_address.sin_port = htons((u_short) port);
}

// Connection
int kl::socket::listen(const int queue_size)
{
    int bind_result = bind(m_socket, (sockaddr*) &m_address, sizeof(m_address));
    if (warning_check(bind_result, "Could not bind socket")) {
        return bind_result;
    }

    int listen_result = ::listen(m_socket, queue_size);
    warning_check(listen_result, "Could not listen on socket");
    return listen_result;
}

kl::socket kl::socket::accept()
{
    int address_length = sizeof(m_address);
    const SOCKET accepted = ::accept(m_socket, (sockaddr*) &m_address, &address_length);
    warning_check(accepted == INVALID_SOCKET, "Could not accept socket");

    socket result = {};
    result.m_socket = accepted;
    return result;
}

int kl::socket::connect()
{
    int result = ::connect(m_socket, (sockaddr*) &m_address, sizeof(m_address));
    warning_check(result, "Could not connect to socket");
    return result;
}

int kl::socket::close() const
{
    return closesocket(m_socket);
}

// Data transfer
int kl::socket::send(const void* data, const int byte_size) const
{
    return ::send(m_socket, (const char*) data, byte_size, NULL);
}

int kl::socket::receive(void* buff, const int byte_size) const
{
    return recv(m_socket, (char*) buff, byte_size, NULL);
}

int kl::socket::exhaust(std::vector<byte>& output, const int buffer_size) const
{
    int received = 0;
    std::vector<byte> receiver_buffer(buffer_size);
    while ((received = receive(receiver_buffer.data(), (int) receiver_buffer.size())) > 0) {
        output.insert(output.end(), receiver_buffer.begin(), receiver_buffer.begin() + received);
    }
    return received;
}
