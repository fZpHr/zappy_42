#include "../include/lib.hpp"

int main(int argc, char** argv) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 1;
        }

        int port = std::stoi(argv[1]);
        zappy::core::Server server(port);
        zappy::utils::Logger::info("Starting server on port " + std::to_string(port));
        server.start();

        return 0;
    } catch (const std::exception& e) {
        zappy::utils::Logger::error("Fatal error: " + std::string(e.what()));
        return 1;
    }
}