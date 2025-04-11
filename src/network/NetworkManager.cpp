#include "../../include/network/NetworkManager.hpp"

NetworkManager::NetworkManager(const size_t main_port, const size_t graphical_port, const size_t &max_clients, const std::vector<std::shared_ptr<Team>> &teams, const std::shared_ptr<Map> &map)
    : main_acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), main_port)),
      graphical_acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), graphical_port)),
      max_clients_(max_clients), teams_(teams), map_(map), map_update_timer_(io_context_) {
}

void NetworkManager::start() {
    accept_connection();
    accept_graphical_connection();
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
    auto socket = std::make_shared<SocketHandler>(boost::asio::ip::tcp::socket(io_context_), teams_);
    main_acceptor_.async_accept(socket->get_socket(), [this, socket](const boost::system::error_code& error) {
        if (!error) {
            if (clients_.size() < max_clients_ ) {
                auto client = std::make_shared<Client>(socket, teams_);
                clients_.push_back(client);
                INFO("Client " + std::to_string(client->get_id()) + " connected from " +
                socket->get_socket().remote_endpoint().address().to_string() + ":" +
                std::to_string(socket->get_socket().remote_endpoint().port()) +
                " to " +
                socket->get_socket().local_endpoint().address().to_string() + ":" +
                std::to_string(socket->get_socket().local_endpoint().port()));
                client->send_message_to("Welcome to the server");
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


void NetworkManager::accept_graphical_connection() {
    auto socket = std::make_shared<SocketHandler>(boost::asio::ip::tcp::socket(io_context_), teams_);
    graphical_acceptor_.async_accept(socket->get_socket(), [this, socket](const boost::system::error_code& error) {
        if (!error) {
            if (graphical_client_) {
                graphical_client_->send_message_to("Another client is taking over");
                graphical_client_->disconnect();
                map_update_timer_.cancel();
            }

            graphical_client_ = std::make_shared<Client>(socket, teams_);
            INFO("Graphical client connected");
            
            graphical_client_->send_message_to(map_->serialize());
            
            start_map_updates();
            
            graphical_client_->receive_message_from();
            
            accept_graphical_connection();
        }
        else {
            ERROR("Graphical client connection error: " + error.message());
            socket->close();
            accept_graphical_connection();
        }
    });
}

void NetworkManager::start_map_updates() {
    map_update_timer_.expires_from_now(boost::posix_time::seconds(MAP_UPDATE_INTERVAL));
    map_update_timer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec && graphical_client_ && graphical_client_->is_connected()) {
            try {
                graphical_client_->send_message_to(map_->serialize());
            }
            catch (const std::exception& e) {
                ERROR("Send error: " + std::string(e.what()));
                graphical_client_->disconnect();
            }
            start_map_updates();
        }
        else if (ec != boost::asio::error::operation_aborted) {
            ERROR("Update error: " + ec.message());
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