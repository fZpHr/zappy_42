#include "../../include/core/Server.hpp"

Server::Server(boost::program_options::variables_map &vm): settings_(&vm), map_(std::make_shared<Map>(vm["width"].as<size_t>(), vm["height"].as<size_t>(), clients_, teams_)) {
    auto team_names = vm["teams"].as<std::vector<std::string>>();
    for (size_t i = 0; i < team_names.size(); i++) {
        DEBUG("Creating team " + std::to_string(i + 1) + ": " + team_names[i]);
        teams_.push_back(std::make_shared<Team>(team_names[i], i, team_names.size() / vm["teams"].as<std::vector<std::string>>().size()));
    }
    map_->placeTeams();
    constexpr size_t GRAPHICAL_PORT = 4244;
    network_manager_ = std::make_unique<NetworkManager>(vm["port"].as<size_t>(), GRAPHICAL_PORT, vm["clients"].as<size_t>(), teams_, map_);
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

    // std::thread map_display_thread([this]() {
    //     while (running_) {
    //         std::ofstream map_file("map_output.txt", std::ios::out | std::ios::trunc);
    //         if (map_file.is_open()) {
    //             map_->printMap(map_file);
    //             map_file.close();
    //         } else {
    //             std::cerr << "Failed to open map_output.txt for writing." << std::endl;
    //         }
    //         std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     }
    // });
    // map_display_thread.detach();

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
