#pragma once
#include "../lib.hpp"

namespace zappy {
    namespace network {

        class NetworkManager {
            public:
                explicit NetworkManager(int port);
                void start();
                void stop();
                void broadcast(const std::string& message);

            private:
                void accept_connection();
                boost::asio::io_context io_context_;
                boost::asio::ip::tcp::acceptor acceptor_;
                std::vector<std::shared_ptr<SocketHandler>> clients_;
        };

    }
}