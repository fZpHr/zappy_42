#include "../../include/lib.hpp"

zappy::core::Client::Client(std::shared_ptr<network::SocketHandler> socket_handler) {
    socket_handler_ = socket_handler;
    id_ = *available_ids_.begin();
    available_ids_.erase(id_);
}

void zappy::core::Client::send_message_to(const std::string& message) {
    socket_handler_->async_write(message, id_);
}

void zappy::core::Client::receive_message_from() {
    socket_handler_->async_read(id_);
}

bool zappy::core::Client::is_connected() const {
    return socket_handler_->is_open();
}

size_t zappy::core::Client::get_id() const {
    return id_;
}

void zappy::core::Client::disconnect() {
    socket_handler_->close();
    available_ids_.insert(id_);
}

void zappy::core::Client::initialize_available_ids(size_t max_clients) {
    for (size_t i = 1; i <= max_clients; ++i) {
        available_ids_.insert(i);
    }
}

std::set<size_t> zappy::core::Client::available_ids_;
