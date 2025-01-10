#include "../../include/network/NetworkManager.hpp"

NetworkManager::NetworkManager(const size_t port , const size_t max_clients, const vector<string> &teams)
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), max_clients_(max_clients), teams_(teams) {
}

void NetworkManager::start() {
    accept_connection();
}

void NetworkManager::stop(){
    io_context_.stop();
}

void NetworkManager::broadcast(const std::string& message){
    for (auto& client : clients_) {
        client->send_message_to(message);
    }
}

void NetworkManager::accept_connection(){
    auto socket = std::make_shared<SocketHandler>(boost::asio::ip::tcp::socket(io_context_));
    acceptor_.async_accept(socket->get_socket(), [this, socket](const boost::system::error_code& error) {
        if (!error) {
            if (clients_.size() < max_clients_) {
                auto client = std::make_shared<Client>(socket, Team(teams_[max_clients_ - clients_.size() - 1],0,0,0));
                clients_.push_back(client);
                INFO("Client " + std::to_string(client->get_id()) + " connected from " +
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
                ERROR("Client trying to connect from " +
                socket->get_socket().remote_endpoint().address().to_string() + ":" +
                std::to_string(socket->get_socket().remote_endpoint().port()) +
                " but server is full");
                socket->async_write("Server is full", -1);
                socket->close();
                accept_connection();
            }
        }
    });
}

void NetworkManager::poll() {
    io_context_.poll();
    clients_.erase(
        std::remove_if(clients_.begin(), clients_.end(),
            [](const auto& client) {
                if (!client->is_connected()) {
                    INFO("Client " + std::to_string(client->get_id()) + " disconnected");
                    client->disconnect();
                    return true;
                }
                return false;
            }
        ),
    clients_.end());
    
}