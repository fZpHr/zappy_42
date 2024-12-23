#include "../../include/lib.hpp"

zappy::core::Server::Server(int port) {
    network_manager_ = std::make_unique<zappy::network::NetworkManager>(port);
    running_ = false;
}

// zappy::core::Server::~Server() = default;

void zappy::core::Server::start() {
    running_ = true;
    zappy::utils::Logger::debug("Starting...");
    network_manager_->start();
    zappy::utils::Logger::debug("Starting aled...");
    while (running_) {
        auto client = network_manager_->accept();
        if (client) {
            clients_.push_back(client);
        }
    }
}

void zappy::core::Server::stop() {
    running_ = false;
    network_manager_->stop();
    for (auto& client : clients_) {
        client->disconnect();
    }
    clients_.clear();
}

bool zappy::core::Server::is_running() const {
    return running_;
}

size_t zappy::core::Server::get_client_count() const {
    return clients_.size();
}
