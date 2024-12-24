#include "../../include/lib.hpp"

zappy::network::NetworkManager::NetworkManager(int port)
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
}

void zappy::network::NetworkManager::start() {
    accept_connection();
}

void zappy::network::NetworkManager::stop(){
    io_context_.stop();
}

void zappy::network::NetworkManager::broadcast(const std::string& message){
    for (auto& client : clients_) {
        client->send_message(message);
    }
}

void zappy::network::NetworkManager::accept_connection(){
    auto socket = std::make_shared<SocketHandler>(tcp::socket(io_context_));
    acceptor_.async_accept(socket->get_socket(), [this, socket](const error_code& error) {
        if (!error) {
            auto client = std::make_shared<zappy::core::Client>(socket);
            clients_.push_back(client);
            client->is_connected();
            client->send_message("Welcome to the server!");
            client->receive_message("Hello from the server!");
            accept_connection();
        }
    });
}

void zappy::network::NetworkManager::poll(){
    io_context_.poll();
}