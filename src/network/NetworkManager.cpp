#include "../../include/network/NetworkManager.hpp"

NetworkManager::NetworkManager(const size_t main_port, const size_t graphical_port, const size_t &max_clients, const std::vector<std::shared_ptr<Team>> &teams, const std::shared_ptr<Map> &map)
    : main_acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), main_port)),
      graphical_acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), graphical_port)),
      max_clients_(max_clients), teams_(teams), map_(map) {
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
                // size_t team_index = client->get_id() % teams_.size();
                // if (team_index >= teams_.size()) {
                //     cout << teams_.size() << " ," << team_index << endl;
                //     ERROR("Team index out of bounds");
                //     socket->close();
                //     accept_connection();
                //     return;
                // }
                // client->set_team(*teams_[team_index]);
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
            if (graphical_client_ && graphical_client_->is_connected()) {
                graphical_client_->send_message_to("New graphical client connected, disconnecting you");
                graphical_client_->disconnect();
            }

            graphical_client_ = std::make_shared<Client>(socket, teams_);
            INFO("Graphical client " + std::to_string(graphical_client_->get_id()) + " connected");

            auto send_map_timer = std::make_shared<boost::asio::steady_timer>(io_context_);
            auto send_map_function = std::make_shared<std::function<void(const boost::system::error_code&)>>();
            
            std::weak_ptr<std::function<void(const boost::system::error_code&)>> weak_function = send_map_function;
            
            *send_map_function = [this, send_map_timer, weak_function](const boost::system::error_code& ec) {
                if (auto shared_function = weak_function.lock()) {
                    if (!ec && graphical_client_ && graphical_client_->is_connected()) {
                        graphical_client_->send_message_to(map_->serialize());
                        send_map_timer->expires_after(std::chrono::seconds(5));
                        if (graphical_client_ && graphical_client_->is_connected()) {
                            send_map_timer->async_wait(*shared_function);
                        } else {
                            INFO("Stopping map updates as the graphical client is disconnected");
                        }
                    }
                }
            };
            
            graphical_client_->send_message_to(map_->serialize());
            graphical_client_->receive_message_from();

            graphical_client_->set_disconnect_callback([send_map_timer]() {
                send_map_timer->cancel();
            });

            accept_graphical_connection();
            graphical_client_->send_message_to(map_->serialize());
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