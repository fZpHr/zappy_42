#pragma once
#include "../core/Client.hpp"
#include "../network/SocketHandler.hpp"
#include "../core/Team.hpp"

class NetworkManager {
    public:
        explicit NetworkManager(const size_t port, const size_t max_clients, const std::vector<std::string> &teams);
        void start();
        void stop();
        std::shared_ptr<Client> accept();
        void broadcast(const std::string& message);
        void poll();

    private:
        void accept_connection();
        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::acceptor acceptor_;
        std::vector<std::shared_ptr<Client>> clients_;
        std::vector<std::string> teams_; 
        const size_t max_clients_;
};
