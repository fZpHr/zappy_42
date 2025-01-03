#pragma once
#include "../lib.hpp"
#include "../core/Client.hpp" 

namespace zappy {
    namespace network {

        class NetworkManager {
            public:
                explicit NetworkManager(const size_t port, const size_t max_clients);
                void start();
                void stop();
                std::shared_ptr<zappy::core::Client> accept();
                void broadcast(const std::string& message);
                void poll();

            private:
                void accept_connection();
                boost::asio::io_context io_context_;
                boost::asio::ip::tcp::acceptor acceptor_;
                std::vector<std::shared_ptr<zappy::core::Client>> clients_;
                const size_t max_clients_;
        };

    }
}