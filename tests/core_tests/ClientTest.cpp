#include <gtest/gtest.h>
#include "../../include/lib.hpp"
#include "../../include/network/SocketHandler.hpp"
#include "../../include/core/Client.hpp"

class ClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        io_context = std::make_shared<boost::asio::io_context>();
        socket = std::make_shared<boost::asio::ip::tcp::socket>(*io_context);
        socket_handler = std::make_shared<zappy::network::SocketHandler>(std::move(*socket));

        zappy::core::Client::initialize_available_ids(100);
    }
    std::shared_ptr<boost::asio::io_context> io_context;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    std::shared_ptr<zappy::network::SocketHandler> socket_handler;
};

TEST_F(ClientTest, UniqueIdGeneration) {
    auto client1 = std::make_shared<zappy::core::Client>(socket_handler);
    auto client2 = std::make_shared<zappy::core::Client>(socket_handler);
    EXPECT_NE(client1->get_id(), client2->get_id());
}

TEST_F(ClientTest, InitialState) {
    auto client = std::make_shared<zappy::core::Client>(socket_handler);
    EXPECT_FALSE(client->is_connected());
}

TEST_F(ClientTest, ConnectionStatus) {
    boost::asio::ip::tcp::acceptor acceptor(*io_context);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 4242);
    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();
    socket_handler->get_socket().connect(endpoint);
    auto client = std::make_shared<zappy::core::Client>(socket_handler);
    EXPECT_TRUE(client->is_connected());
    client->disconnect();
    EXPECT_FALSE(client->is_connected());
}

TEST_F(ClientTest, Disconnection) {
    auto client = std::make_shared<zappy::core::Client>(socket_handler);
    client->disconnect();
    EXPECT_FALSE(client->is_connected());
}