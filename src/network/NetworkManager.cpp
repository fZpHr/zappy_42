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
        client->send_message_to(message);
    }
}

void zappy::network::NetworkManager::accept_connection(){
    auto socket = std::make_shared<SocketHandler>(boost::asio::ip::tcp::socket(io_context_));
    acceptor_.async_accept(socket->get_socket(), [this, socket](const error_code& error) {
        if (!error) {
            if (clients_.size() < MAX_CLIENTS) {
                auto client = std::make_shared<zappy::core::Client>(socket);
                clients_.push_back(client);
                ZAPPY_INFO("Client " + std::to_string(client->get_id()) + " connected from " +
                socket->get_socket().remote_endpoint().address().to_string() + ":" +
                std::to_string(socket->get_socket().remote_endpoint().port()) +
                " to " +
                socket->get_socket().local_endpoint().address().to_string() + ":" +
                std::to_string(socket->get_socket().local_endpoint().port()));
                client->send_message_to("Welcome to the server!");
                client->receive_message_from();
                accept_connection();
            }
            else {
                ZAPPY_ERROR("Client trying to connect from " +
                socket->get_socket().remote_endpoint().address().to_string() + ":" +
                std::to_string(socket->get_socket().remote_endpoint().port()) +
                " but server is full");
                socket->async_write("Server is full");
                socket->close();
                accept_connection();
            }
        }
    });
}

void zappy::network::NetworkManager::poll() {
    io_context_.poll();
    clients_.erase(
        std::remove_if(clients_.begin(), clients_.end(),
            [](const auto& client) {
                if (!client->is_connected()) {
                    ZAPPY_INFO("Client " + std::to_string(client->get_id()) + " disconnected");
                    client->disconnect();
                    return true;
                }
                return false;
            }
        ),
    clients_.end());
    
}