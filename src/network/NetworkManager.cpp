#include "../../include/lib.hpp"

zappy::network::NetworkManager::NetworkManager(int port)
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
}

void zappy::network::NetworkManager::start() {
    accept_connection();
    zappy::utils::Logger::info("Starting event loop...");
    
    while (!io_context_.stopped()) {
        io_context_.poll_one();
    }
}

void zappy::network::NetworkManager::stop(){
    io_context_.stop();
}

void zappy::network::NetworkManager::broadcast(const std::string& message){
    for (auto& client : clients_) {
        client->async_write(message);
    }
}

std::shared_ptr<zappy::core::Client> zappy::network::NetworkManager::accept(){
auto socket = std::make_shared<SocketHandler>(tcp::socket(io_context_));
    acceptor_.async_accept(socket->get_socket(), [this, socket](const error_code& error) {
        if (!error) {
            clients_.push_back(socket);
            accept_connection();
        }
    });
    return nullptr;
}

void zappy::network::NetworkManager::accept_connection(){
    auto socket = std::make_shared<SocketHandler>(tcp::socket(io_context_));
    acceptor_.async_accept(socket->get_socket(), [this, socket](const error_code& error) {
        if (!error) {
            clients_.push_back(socket);
            socket->async_read();
            accept_connection();
        }
    });
}