#include <gtest/gtest.h>
#include "core/Server.hpp"
#include <thread>
#include <chrono>

class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        server = std::make_unique<zappy::core::Server>(test_port);
    }

    void TearDown() override {
        if (server && server->is_running()) {
            server->stop();
        }
    }

    static const int test_port = 4242;
    std::unique_ptr<zappy::core::Server> server;
};

TEST_F(ServerTest, ServerStartStop) {
    ASSERT_FALSE(server->is_running());
    
    std::thread server_thread([this]() {
        server->start();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_TRUE(server->is_running());

    server->stop();
    server_thread.join();
    ASSERT_FALSE(server->is_running());
}