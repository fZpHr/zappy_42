#include "../../include/lib.hpp"

zappy::network::SocketHandler::SocketHandler(boost::asio::ip::tcp::socket socket, size_t id)
    : socket_(std::move(socket)) {
}

void zappy::network::SocketHandler::async_read() {
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_buffer_), '\n', [this, self](const error_code& error
        , size_t bytes_transferred) {
        if (error) {
            close();
            return;
        }
        std::string message = read_buffer_;
        read_buffer_.clear();
        async_read();
    });
}

void zappy::network::SocketHandler::async_write(const std::string& message) {
    auto self = shared_from_this();
    boost::asio::async_write(socket_, boost::asio::buffer(message + '\n'), [this, self](const error_code& error, size_t bytes_transferred) {
        if (error) {
            close();
            return;
        }
    });
}

bool zappy::network::SocketHandler::is_open() const {
    return socket_.is_open();
}

void zappy::network::SocketHandler::close() {
    socket_.close();
}

boost::asio::ip::tcp::socket& zappy::network::SocketHandler::get_socket() {
    return socket_;
}

