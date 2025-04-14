#include <gtest/gtest.h>
#include "../../include/lib.hpp"
#include "../../include/core/Server.hpp"


class ServerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            vm = boost::program_options::variables_map();
            vm.insert(std::make_pair("port", boost::program_options::variable_value(static_cast<size_t>(4243), false)));
            vm.insert(std::make_pair("width", boost::program_options::variable_value(static_cast<size_t>(20), false)));
            vm.insert(std::make_pair("height", boost::program_options::variable_value(static_cast<size_t>(20), false)));
            
            std::vector<std::string> team_names = {"Team1", "Team2"};
            vm.insert(std::make_pair("teams", boost::program_options::variable_value(team_names, false)));
            vm.insert(std::make_pair("clients", boost::program_options::variable_value(static_cast<size_t>(10), false)));
            vm.insert(std::make_pair("time", boost::program_options::variable_value(static_cast<size_t>(10), false)));
        }
        
        boost::asio::io_context io_context;
        boost::program_options::variables_map vm;
};

TEST_F(ServerTest, CommandLineOptions) {
    int argc = 14;
    const char* argv[] = {
        "server",
        "--port", "4243",
        "--width", "20",
        "--height", "20",
        "--teams", "Team1", "Team2",
        "--clients", "4",
        "--time", "10"
    };

    try {
        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "show help message")
            ("port,p", boost::program_options::value<size_t>()->notifier([](size_t value) {
                if (value < 1024 || value > 49151) throw boost::program_options::error("Invalid port number");}
            )->required(), "port number")
            ("width,x", boost::program_options::value<size_t>()->notifier([](size_t value) {
                if (value < 10 || value > 30) throw boost::program_options::error("Invalid world width");}
            )->required(), "world width")
            ("height,y", boost::program_options::value<size_t>()->notifier([](size_t value) {
                if (value < 10 || value > 30) throw boost::program_options::error("Invalid world height");}
            )->required(), "world height")
            ("teams,n", boost::program_options::value<std::vector<std::string>>()->notifier([](const std::vector<std::string>& value) {
                if (value.size() < 2 || value.size() > 18) throw boost::program_options::error("At least 2 teams and at max 18 teams are required");
                for (const auto& team : value) {
                    if (team.empty()) throw boost::program_options::error("Invalid team name");
                }}
            )->multitoken()->required(), "team names")
            ("clients,c", boost::program_options::value<size_t>()->notifier([](size_t value) {
                if (value < 2 || value > 100) throw boost::program_options::error("At least two clients and at max 100 clients are required");}
            )->required(), "number of clients authorized")
            ("time,t", boost::program_options::value<size_t>()->notifier([](size_t value) {
                if (value < 1) throw boost::program_options::error("Invalid time unit");}
            )->required(), "time unit");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help") || argc == 1) {
            std::cout << desc << std::endl;
            return;
        }

        try {
            boost::program_options::notify(vm);
        } catch (const boost::program_options::error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return;
        }

        std::vector<std::string> teams = vm["teams"].as<std::vector<std::string>>();
        size_t clients = vm["clients"].as<size_t>();
        size_t num_teams = teams.size();
        if (clients < num_teams) {
            std::cerr << "Number of clients must be greater than or equal to the number of teams" << std::endl;
            return;
        }
        if (clients % num_teams != 0) {
            size_t adjusted_clients = (clients / num_teams) * num_teams;
            std::cerr << "Number of clients must be a multiple of the number of teams. Adjusting to " << adjusted_clients << " clients." << std::endl;
            clients = adjusted_clients;
        }

        Server server(vm);
    
        std::atomic<bool> server_started = false;
        std::thread server_thread([&]() {
            server_started = true;
            server.start();
        });
        
        auto start = std::chrono::steady_clock::now();
        while (!server_started && 
            std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
        server.stop();
        
        if (server_thread.joinable()) {
            server_thread.join();
        }
        
        EXPECT_TRUE(server_started);
    } catch (const std::exception& e) {
        FAIL() << "Exception: " << e.what();
    }
}
TEST_F(ServerTest, ClientConnection) {
    Server server(vm);
    std::thread server_thread([&server]() { 
        server.start(); 
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    boost::asio::io_context client_io;
    boost::asio::ip::tcp::socket client_socket(client_io);
    EXPECT_NO_THROW({
        client_socket.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), 4243));
    });
    EXPECT_TRUE(client_socket.is_open());
    
    client_socket.close();
    server.stop();
    server_thread.join();
}

TEST_F(ServerTest, ClientDisconnection) {
    Server server(vm);
    std::thread server_thread([&server]() { 
        server.start(); 
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    boost::asio::io_context client_io;
    boost::asio::ip::tcp::socket client_socket(client_io);
    client_socket.connect(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string("127.0.0.1"), 4243));
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(server.get_client_count(), 1);
    client_socket.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(server.get_client_count(), 0);
    
    server.stop();
    server_thread.join();
}

TEST_F(ServerTest, ClientCommands) {
    Server server(vm);
    std::thread server_thread([&server]() { 
        server.start(); 
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    boost::asio::io_context client_io;
    boost::asio::ip::tcp::socket client_socket(client_io);
    client_socket.connect(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string("127.0.0.1"), 4243));
    
    std::string command = "Team1\n";
    boost::asio::write(client_socket, boost::asio::buffer(command));
    
    char response[1024] = {0};
    boost::system::error_code error;
    size_t bytes = client_socket.read_some(boost::asio::buffer(response), error);
    std::string resp_str(response, bytes);
    printf("Response: %s\n", resp_str.c_str());
    
    EXPECT_TRUE(resp_str.find("Welcome to the server\n") != std::string::npos);
    
    client_socket.close();
    server.stop();
    server_thread.join();
}

TEST_F(ServerTest, LoadTest) {
    Server server(vm);
    std::thread server_thread([&server]() { 
        server.start(); 
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    const int NUM_CLIENTS = 10;
    boost::asio::io_context client_io;
    std::vector<std::unique_ptr<boost::asio::ip::tcp::socket>> clients;
    
    for (int i = 0; i < NUM_CLIENTS; i++) {
        auto socket = std::make_unique<boost::asio::ip::tcp::socket>(client_io);
        socket->connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), 4243));
        clients.push_back(std::move(socket));
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(server.get_client_count(), NUM_CLIENTS);
    
    server.stop();
    for (auto& socket : clients) {
        socket->close();
    }
    server_thread.join();
}

TEST_F(ServerTest, MemoryLeakCheck) {
    Server server(vm);
    std::thread server_thread([&server]() { 
        server.start(); 
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    boost::asio::io_context client_io;
    
    for (int i = 0; i < 10; i++) {
        boost::asio::ip::tcp::socket tmp(client_io);
        tmp.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), 4243));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        tmp.close();
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    EXPECT_EQ(server.get_client_count(), 0);
    
    server.stop();
    server_thread.join();
}