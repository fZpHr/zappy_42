#include "../../include/network/SocketHandler.hpp"


void SocketHandler::async_read(std::shared_ptr<Client> client) {
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_buffer_), '\n', [this, self, client](const error_code& error
        , size_t bytes_transferred) {
        if (error) {
            close();
            return;
        }
        (void)bytes_transferred;
        string message = read_buffer_;
        message.pop_back();
        LOG("Client " + to_string(client->get_id()) + " sent: " + message);
        if (command_handler_.execute(message) == 2) {
            for (auto &team : teams_) {
                if (team->get_name() == message) {
                    client->set_team(*team);
                    client->send_message_to("Team " + message + " has " + to_string(team->get_max_size()) + " places");
                    client->send_message_to("Team position is " + to_string(team->get_x()) + " " + to_string(team->get_y()));   
                    break;
                }
            }
        }
        read_buffer_.clear();
        async_read(client);
    });
}

void SocketHandler::async_write(const std::string &message, const int &id) {
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