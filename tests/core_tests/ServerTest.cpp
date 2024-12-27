#include <gtest/gtest.h>
#include "../../include/lib.hpp"

class ServerTest : public ::testing::Test {
protected:
    boost::asio::io_context io_context;
};

TEST_F(ServerTest, CommandLineOptions) {
    int argc = 11;
    const char* argv[] = {
        "server",
        "--port", "4242",
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

        zappy::core::Server server(vm);
        zappy::utils::Logger::info("Starting server on port " + std::to_string(vm["port"].as<size_t>()) + " with " + std::to_string(clients) + " slots" + " and " + std::to_string(num_teams) + " teams");
        server.start();

    } catch (const std::exception& e) {
        zappy::utils::Logger::error("Error: " + std::string(e.what()));
    }
}