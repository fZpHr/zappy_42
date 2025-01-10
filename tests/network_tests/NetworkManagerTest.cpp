#include <gtest/gtest.h>
#include "../../include/lib.hpp"

class NetworkManagerTest : public ::testing::Test {
protected:
   const int TEST_PORT = 4242;
};

TEST_F(NetworkManagerTest, ConstructorAndStartStop) {
   NetworkManager manager(TEST_PORT, 10);
   std::thread manager_thread([&manager]() {
       manager.start();
   });
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   manager.stop();
   manager_thread.join();
   EXPECT_NO_THROW();
}

TEST_F(NetworkManagerTest, ClientAcceptance) {
   NetworkManager manager(TEST_PORT, 10);
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