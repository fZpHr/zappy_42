#include "../../include/lib.hpp"

zappy::core::Server::Server(int port) {
    network_manager_ = std::make_unique<zappy::network::NetworkManager>(port);
    running_ = false;
    size_t sizeC = MAX_CLIENTS;
    zappy::core::Client::initialize_available_ids(MAX_CLIENTS);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGPIPE, SIG_IGN);
}

std::atomic<bool> zappy::core::Server::signal_received(false);

void zappy::core::Server::signal_handler(int signal) {
    signal_received.store(true);
}

void zappy::core::Server::start() {
    network_manager_->start();
    running_ = true;
    while (running_) {
        if (signal_received) {
            ZAPPY_INFO("Received signal. Stopping server...");
            stop();
            break;
        }
        network_manager_->poll();
        clients_.erase(std::remove_if(clients_.begin(), clients_.end(), [](const auto& client) {
            return !client->is_connected();
        }), clients_.end());
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
