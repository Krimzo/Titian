#pragma once

#include "math/math.h"


namespace kl {
    class socket
    {
        sockaddr_in m_address = {};
        SOCKET m_socket = {};

    public:
        static const std::string self;

        // Init
        socket();
        socket(int port);
        socket(const std::string& address, int port);
        ~socket();

        // Properties
        SOCKET id() const;

        std::string address() const;
        int set_address(const std::string& address);

        int port() const;
        void set_port(int port);

        // Connection
        int listen(int queue_size);
        socket accept();
        int connect();
        int close() const;

        // Data transfer
        int send(const void* data, int byte_size) const;
        int receive(void* buff, int byte_size) const;
        int exhaust(std::vector<byte>& output, int buffer_size = 16384) const;

        template <typename T>
        int send(const T& obj) const
        {
            return send(&obj, sizeof(T));
        }

        template <typename T>
        int receive(T& obj) const
        {
            return receive(&obj, sizeof(T));
        }
    };
}
