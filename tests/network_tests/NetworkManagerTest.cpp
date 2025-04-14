#include <gtest/gtest.h>
#include "../../include/lib.hpp"
#include "../../include/network/NetworkManager.hpp"

class NetworkManagerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            clients_ = std::vector<std::shared_ptr<Client>>();
            teams_ = std::vector<std::shared_ptr<Team>>();
            
            map_ = std::make_shared<Map>(20, 20, clients_, teams_);
        }
        
        const int TEST_PORT = 4243;
        const int TEST_GRAPHICAL_PORT = 4244;
        std::vector<std::shared_ptr<Client>> clients_;
        std::vector<std::shared_ptr<Team>> teams_;
        std::shared_ptr<Map> map_;
};
    
TEST_F(NetworkManagerTest, ConstructorAndStartStop) {
    NetworkManager manager(TEST_PORT, TEST_GRAPHICAL_PORT, 10, teams_, map_, clients_);
    std::thread manager_thread([&manager]() {
        manager.start();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    manager.stop();
    manager_thread.join();
    EXPECT_NO_THROW();
}

TEST_F(NetworkManagerTest, ClientAcceptance) {
    NetworkManager manager(TEST_PORT, TEST_GRAPHICAL_PORT, 10, teams_, map_, clients_);
    std::thread manager_thread([&manager]() {
        manager.start();
    });
    
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket client(io);
    client.connect(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string("127.0.0.1"), TEST_PORT));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    manager.stop();
    manager_thread.join();
}
