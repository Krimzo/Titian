#pragma once

#include "math/math.h"


namespace kl {
    class Socket
    {
    public:
        using ID = uint64_t;
        static const std::string SELF;

    private:
        sockaddr_in m_address = {};
        ID m_socket = {};

    public:
        Socket();
        Socket(int port);
        Socket(const std::string_view& address, int port);
        ~Socket();

        Socket(const Socket&) = delete;
        Socket(Socket&&) = delete;

        void operator=(const Socket&) = delete;
        void operator=(Socket&&) = delete;

        // Properties
        ID id() const;

        std::string address() const;
        int set_address(const std::string_view& address);

        int port() const;
        void set_port(int port);

        // Connection
        int listen(int queue_size);
        void accept(Socket* socket);
        int connect();
        int close();

        // Data transfer
        int send(const void* data, int byte_size) const;
        int receive(void* buff, int byte_size) const;
        int exhaust(std::vector<byte>* output, int buffer_size = 16384) const;

        template <typename T>
        bool send(const T& obj) const
        {
            const int sent_size = send(&obj, sizeof(T));
            return sent_size == sizeof(T);
        }

        template <typename T>
        bool receive(T& obj) const
        {
            const int received_size = receive(&obj, sizeof(T));
            return received_size == sizeof(T);
        }
    };
}
