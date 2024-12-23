#include <gtest/gtest.h>
#include "../../include/lib.hpp"

class ServerTest : public ::testing::Test {
protected:
   boost::asio::io_context io_context;
};

TEST_F(ServerTest, StartStop) {
    zappy::core::Server server(4242);
    std::thread server_thread([&server]() {
        server.start();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    server.stop();
    server_thread.join();
}