#include <gtest/gtest.h>
#include "../../include/lib.hpp"
#include "../../include/network/SocketHandler.hpp"
#include "../../include/core/Client.hpp"
#include "../../include/core/Team.hpp"

class ClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        io_context = std::make_shared<boost::asio::io_context>();
        socket = std::make_shared<boost::asio::ip::tcp::socket>(*io_context);
        socket_handler = std::make_shared<SocketHandler>(std::move(*socket), teams);

        Client::initialize_available_ids(100);
    }
    std::shared_ptr<boost::asio::io_context> io_context;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    std::shared_ptr<SocketHandler> socket_handler;
    std::vector<std::shared_ptr<Team>> teams;
};

TEST_F(ClientTest, UniqueIdGeneration) {
    auto team1 = std::make_shared<Team>("team1", 0, 0);
    std::vector<std::shared_ptr<Team>> test_teams = {team1};
    
    auto client1 = std::make_shared<Client>(socket_handler, test_teams);
    auto client2 = std::make_shared<Client>(socket_handler, test_teams);
    EXPECT_NE(client1->get_id(), client2->get_id());
}

TEST_F(ClientTest, InitialState) {
    auto team1 = std::make_shared<Team>("team1", 0, 0);
    std::vector<std::shared_ptr<Team>> test_teams = {team1};
    auto client = std::make_shared<Client>(socket_handler, test_teams);
    EXPECT_FALSE(client->is_connected());
}

TEST_F(ClientTest, ConnectionStatus) {
    auto team1 = std::make_shared<Team>("team1", 0, 0);
    std::vector<std::shared_ptr<Team>> test_teams = {team1};
    
    boost::asio::ip::tcp::acceptor acceptor(*io_context);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 4243);
    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();
    socket_handler->get_socket().connect(endpoint);
    auto client = std::make_shared<Client>(socket_handler, test_teams);
    EXPECT_TRUE(client->is_connected());
    client->disconnect();
    EXPECT_FALSE(client->is_connected());
}

TEST_F(ClientTest, Disconnection) {
    auto team1 = std::make_shared<Team>("team1", 0, 0);
    std::vector<std::shared_ptr<Team>> test_teams = {team1};
    auto client = std::make_shared<Client>(socket_handler, test_teams);
    client->disconnect();
    EXPECT_FALSE(client->is_connected());
}