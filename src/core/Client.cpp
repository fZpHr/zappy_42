#include "../../include/lib.hpp"

zappy::core::Client::Client(std::shared_ptr<network::SocketHandler> socket_handler)
    : socket_handler_(socket_handler), id_(next_id_++) {
}

void zappy::core::Client::send_message(const std::string& message) {
    zappy::utils::Logger::info("Client " + std::to_string(id_) + " sent message: " + message);
    socket_handler_->async_write(message);
}

bool zappy::core::Client::is_connected() const {
    zappy::utils::Logger::info("Client " + std::to_string(id_) + " is connected");
    return socket_handler_->is_open();
}

size_t zappy::core::Client::get_id() const {
    return id_;
}

void zappy::core::Client::disconnect() {
    zappy::utils::Logger::info("Client " + std::to_string(id_) + " disconnected");
    socket_handler_->close();
}

size_t zappy::core::Client::next_id_ = 0;
