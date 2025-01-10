#include "../../include/core/Client.hpp"

Client::Client(std::shared_ptr<SocketHandler> socket_handler, Team crr_team)  {
    x = crr_team.get_x();
    y = crr_team.get_y();
    team_name = crr_team.get_name();
    socket_handler_ = socket_handler;
    id_ = *available_ids_.begin();
    available_ids_.erase(id_);
}

void Client::send_message_to(const std::string& message) {
    socket_handler_->async_write(message, id_);
}

void Client::receive_message_from() {
    socket_handler_->async_read(id_);
}

bool Client::is_connected() const {
    return socket_handler_->is_open();
}

size_t Client::get_id() const {
    return id_;
}

void Client::disconnect() {
    socket_handler_->close();
    available_ids_.insert(id_);
}

void Client::initialize_available_ids(size_t max_clients) {
    for (size_t i = 1; i <= max_clients; ++i) {
        available_ids_.insert(i);
    }
}

std::set<size_t> Client::available_ids_;
