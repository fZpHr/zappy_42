#include "../../include/core/Server.hpp"

Server::Server(boost::program_options::variables_map &vm): settings_(&vm), map_(vm["width"].as<size_t>(), vm["height"].as<size_t>()) {
    network_manager_ = std::make_unique<NetworkManager>(vm["port"].as<size_t>(), vm["clients"].as<size_t>(), vm["teams"].as<std::vector<std::string>>());
    Client::initialize_available_ids(vm["clients"].as<size_t>());
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGPIPE, SIG_IGN);
}

std::atomic<bool> Server::signal_received(false);

void Server::signal_handler(int signal) {
    (void)signal;
    signal_received.store(true);
}

void Server::start() {
    network_manager_->start();
    running_ = true;
    while (running_) {
        if (signal_received) {
            INFO("Received signal. Stopping server...");
            stop();
            break;
        }
        network_manager_->poll();
        clients_.erase(std::remove_if(clients_.begin(), clients_.end(), [](const auto& client) {
            return !client->is_connected();
        }), clients_.end());
    }
}

void Server::stop() {
    running_ = false;
    network_manager_->stop();
    for (auto& client : clients_) {
        client->disconnect();
    }
    clients_.clear();
}

bool Server::is_running() const {
    return running_;
}

size_t Server::get_client_count() const {
    return clients_.size();
}
