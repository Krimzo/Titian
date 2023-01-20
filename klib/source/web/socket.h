#pragma once

#include "window/system_handler.h"
#include "math/math.h"


namespace kl {
    class socket
    {
    public:
        inline static const std::string self_ip = "127.0.0.1";
        static void init_utility();
        static void free_utility();
        
    private:
        sockaddr_in address_ = {};
        size_t socket_ = {};

    public:
        socket();
        socket(const std::string& address, int port);
        explicit socket(size_t socket);
        ~socket();

        socket(const socket&) = delete;
        socket(const socket&&) = delete;

        void operator=(const socket&) = delete;
        void operator=(const socket&&) = delete;

        size_t get_socket() const;

        void close() const;

        [[nodiscard]] std::string get_address() const;

        void set_address(const std::string& address);

        [[nodiscard]] int get_port() const;

        void set_port(int port);

        void listen(int queue_size);

        [[nodiscard]] socket accept();

        void connect();

        int send(const void* data, int byte_size) const;

        int receive(void* buff, int byte_size) const;

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
