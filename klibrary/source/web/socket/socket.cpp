#include "klibrary.h"


// Init
namespace kl_ignored {
    [[maybe_unused]] static const int DONT_CARE = []()
    {
        WSADATA wsa_data = {};
        const int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        kl::assert(result == 0, "Failed to initialize WSA");
        return result;
    }();
}

// Static
const std::string kl::Socket::self = "127.0.0.1";

// Construct
kl::Socket::Socket()
{
    m_socket = ::socket(AF_INET, SOCK_STREAM, NULL);
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    verify(m_socket != INVALID_SOCKET, "Failed to create socket");
}

kl::Socket::Socket(const int port)
    : Socket(self, port)
{}

kl::Socket::Socket(const std::string& address, const int port)
    : Socket()
{
    set_address(address);
    set_port(port);
}

kl::Socket::~Socket()
{
    this->close();
}

// Properties
SOCKET kl::Socket::id() const
{
    return m_socket;
}

std::string kl::Socket::address() const
{
    char buffer[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &m_address.sin_addr, buffer, INET_ADDRSTRLEN);
    return { buffer };
}

int kl::Socket::set_address(const std::string& address)
{
    const int result = inet_pton(AF_INET, address.c_str(), &m_address.sin_addr);
    verify(result == 1, format("Could not convert address ", address));
    return result;
}

int kl::Socket::port() const
{
    return (int) ntohs(m_address.sin_port);
}

void kl::Socket::set_port(const int port)
{
    m_address.sin_port = htons((u_short) port);
}

// Connection
int kl::Socket::listen(const int queue_size)
{
    const int bind_result = bind(m_socket, (sockaddr*) &m_address, sizeof(m_address));
    if (!verify(bind_result == 0, "Could not bind socket")) {
        return bind_result;
    }

    const int listen_result = ::listen(m_socket, queue_size);
    verify(listen_result == 0, "Could not listen on socket");
    return listen_result;
}

kl::Socket kl::Socket::accept()
{
    int address_length = sizeof(m_address);
    const SOCKET accepted = ::accept(m_socket, (sockaddr*) &m_address, &address_length);
    verify(accepted != INVALID_SOCKET, "Could not accept socket");

    Socket result = {};
    result.m_socket = accepted;
    return result;
}

int kl::Socket::connect()
{
    const int result = ::connect(m_socket, (sockaddr*) &m_address, sizeof(m_address));
    verify(result == 0, "Could not connect to socket");
    return result;
}

int kl::Socket::close() const
{
    return closesocket(m_socket);
}

// Data transfer
int kl::Socket::send(const void* data, const int byte_size) const
{
    return ::send(m_socket, (const char*) data, byte_size, NULL);
}

int kl::Socket::receive(void* buff, const int byte_size) const
{
    return recv(m_socket, (char*) buff, byte_size, NULL);
}

int kl::Socket::exhaust(std::vector<byte>& output, const int buffer_size) const
{
    int received = 0;
    std::vector<byte> receiver_buffer(buffer_size);
    while ((received = receive(receiver_buffer.data(), (int) receiver_buffer.size())) > 0) {
        output.insert(output.end(), receiver_buffer.begin(), receiver_buffer.begin() + received);
    }
    return received;
}
