#include "../../include/network/SocketHandler.hpp"

SocketHandler::SocketHandler(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket)), command_handler_() {
}

void SocketHandler::async_read(const size_t &id) {
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_buffer_), '\n', [this, self, id](const error_code& error
        , size_t bytes_transferred) {
        if (error) {
            close();
            return;
        }
        (void)bytes_transferred;
        string message = read_buffer_;
        message.pop_back();
        TEAM("Client " + to_string(id) + ": " + message);
        command_handler_.execute(message);
        read_buffer_.clear();
        async_read(id);
    });
}

void SocketHandler::async_write(const std::string& message, const int &id) {
    auto self = shared_from_this();
    boost::asio::async_write(socket_, boost::asio::buffer(message + '\n'), [this, self, id, message](const error_code& error, size_t bytes_transferred) {
        if (error) {
            close();
            return;
        }
        (void)bytes_transferred;
        LOG("Sending message to Client " + to_string(id) + ": " + message);
    });
}

bool SocketHandler::is_open() const {
    return socket_.is_open();
}

void SocketHandler::close() {
    socket_.close();
}

boost::asio::ip::tcp::socket& SocketHandler::get_socket() {
    return socket_;
}

SocketHandler::~SocketHandler() {
    close();
}