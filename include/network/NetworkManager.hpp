#pragma once
#include "../core/Client.hpp"
#include "../network/SocketHandler.hpp"
#include "../core/Team.hpp"
#include "../core/Map.hpp"
#include "../lib.hpp"

class NetworkManager {
    public:
        NetworkManager(const size_t main_port, const size_t graphical_port, const size_t &max_clients, const std::vector<std::shared_ptr<Team>> &teams, const std::shared_ptr<Map> &map);
        void start();
        void stop();
        std::shared_ptr<Client> accept();
        void broadcast(const std::string& message);
        void poll();
        

    private:
        void accept_connection();
        void accept_graphical_connection();
        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::acceptor main_acceptor_;
        boost::asio::ip::tcp::acceptor graphical_acceptor_;
        const size_t max_clients_;
        std::vector<std::shared_ptr<Client>> clients_;
        std::shared_ptr<Client> graphical_client_;
        std::vector<std::shared_ptr<Team>> teams_;
        std::shared_ptr<Map> map_;
};
