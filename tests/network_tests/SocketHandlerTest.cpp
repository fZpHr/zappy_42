#include <gtest/gtest.h>
#include "../../include/lib.hpp"
#include "../../include/network/SocketHandler.hpp"

class SocketHandlerTest : public ::testing::Test {
protected:
   boost::asio::io_context io_context;
};

TEST_F(SocketHandlerTest, SocketOperations) {
    boost::asio::ip::tcp::acceptor acceptor(io_context, 
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4243));
    acceptor.listen();
    
    std::vector<std::shared_ptr<Team>> teams;
    
    auto socket = std::make_shared<SocketHandler>(
        boost::asio::ip::tcp::socket(io_context), teams);
    
    socket->get_socket().connect(
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), 4243));

    EXPECT_TRUE(socket->is_open());
    socket->close();
    EXPECT_FALSE(socket->is_open());
}

TEST_F(SocketHandlerTest, AsyncReadWrite) {
    std::vector<std::shared_ptr<Team>> teams;
    
    auto socket = std::make_shared<SocketHandler>(
        boost::asio::ip::tcp::socket(io_context), teams);
    
    std::string test_message = "Hello";
    EXPECT_NO_THROW({
        socket->async_write(test_message, 0);
        socket->async_read(0);
    });
}
