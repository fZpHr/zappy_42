#include <gtest/gtest.h>
#include "../../include/lib.hpp"

class SocketHandlerTest : public ::testing::Test {
protected:
   boost::asio::io_context io_context;
};

TEST_F(SocketHandlerTest, SocketOperations) {
    boost::asio::ip::tcp::acceptor acceptor(io_context, 
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4242));
    acceptor.listen();

    auto socket = std::make_shared<SocketHandler>(
        boost::asio::ip::tcp::socket(io_context));
    socket->get_socket().connect(
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), 4242));

    EXPECT_TRUE(socket->is_open());
    socket->close();
    EXPECT_FALSE(socket->is_open());
}

TEST_F(SocketHandlerTest, AsyncReadWrite) {
   auto socket = std::make_shared<SocketHandler>(
       boost::asio::ip::tcp::socket(io_context));
   
   std::string test_message = "Hello";
   EXPECT_NO_THROW({
       socket->async_write(test_message, 0);
       socket->async_read(0);
   });
}
