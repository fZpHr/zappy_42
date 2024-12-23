#include "../../include/lib.hpp"

void zappy::utils::Logger::info(const std::string& message) {
    log("INFO", message);
}

void zappy::utils::Logger::error(const std::string& message) {
    log("ERROR", message);
}

void zappy::utils::Logger::debug(const std::string& message) {
    log("DEBUG", message);
}

void zappy::utils::Logger::log(const std::string& level, const std::string& message) {
    std::cout << "[" << level << "] " << message << std::endl;
}