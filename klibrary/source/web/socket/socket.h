#pragma once

#include "math/math.h"


namespace kl {
    struct Address : public sockaddr_in
    {
        Address();
        
        std::string address() const;
        int set_address(const std::string_view& address);

        int port() const;
        void set_port(int port);
    };
}

namespace kl {
    class Socket
    {
    public:
        using ID = uint64_t;
        static const std::string SELF;

    private:
        Address m_address = {};
        ID m_socket = {};

    public:
        Socket(bool udp);
        ~Socket();

        Socket(const Socket&) = delete;
        Socket(Socket&&) = delete;

        void operator=(const Socket&) = delete;
        void operator=(Socket&&) = delete;

        // Properties
        ID id() const;
        operator bool() const;

        std::string address() const;
        int set_address(const std::string_view& address);

        int port() const;
        void set_port(int port);

        // Connection
        int bind();
        int listen(int queue_size);
        void accept(Socket* socket);
        int connect();

        // TCP transfer
        int send(const void* data, int byte_size) const;
        int receive(void* buff, int byte_size) const;

        template <typename T>
        bool send(const T& obj) const
        {
            return send(&obj, sizeof(T)) == sizeof(T);
        }

        template <typename T>
        bool receive(T* obj) const
        {
            return receive(obj, sizeof(T)) == sizeof(T);
        }

        template <typename T>
        T receive() const
        {
            T t{};
            receive(&t);
            return t;
        }

        // UDP transfer
        int send_to(const void* data, int byte_size, const Address& address) const;
        int receive_from(void* buff, int byte_size, Address* address) const;

        template <typename T>
        bool send_to(const T& obj, const Address& address) const
        {
            return send_to(&obj, sizeof(T), address) == sizeof(T);
        }

        template <typename T>
        bool receive_from(T* obj, Address* address) const
        {
            return receive_from(obj, sizeof(T), address) == sizeof(T);
        }

        template <typename T>
        T receive_from(Address* address) const
        {
            T t{};
            receive_from(&t, address);
            return t;
        }

        // Helper
        int exhaust(std::vector<byte>* output, int buffer_size = 16384) const;
    };
}

namespace kl_ignored {
    [[maybe_unused]] inline static const int SOCKET_DONT_CARE = []()
    {
        WSADATA wsa_data{};
        const int result = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
        kl::assert(result == 0, "Failed to initialize WSA");
        return result;
    }();
}
