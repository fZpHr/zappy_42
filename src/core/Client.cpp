#include "../../include/core/Client.hpp"

Client::Client(std::shared_ptr<SocketHandler> socket_handler, std::vector<std::shared_ptr<Team>>& teams)
    : socket_handler_(socket_handler), teams_(teams) {
    id_ = *available_ids_.begin();
    available_ids_.erase(id_);
}

void Client::set_team(Team team) {
    x = team.get_x();
    y = team.get_y();
    team_name = team.get_name();
}

void Client::send_message_to(const std::string& message) {
    socket_handler_->async_write(message, id_);
}

void Client::receive_message_from() {
    auto shared_this = shared_from_this();
    socket_handler_->async_read(shared_this); 
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

string Client::get_team_name() const {
    return team_name;
}

std::set<size_t> Client::available_ids_;
