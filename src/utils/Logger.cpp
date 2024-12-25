#include "../../include/lib.hpp"

void zappy::utils::Logger::info(const std::string &message) {
    log_handler("INFO", message);
}

void zappy::utils::Logger::error(const std::string &message) {
    log_handler("ERROR", message);
}

void zappy::utils::Logger::debug(const std::string &message) {
    log_handler("DEBUG", message);
}

void zappy::utils::Logger::log(const std::string &message) {
    log_handler("LOG", message);
}

void zappy::utils::Logger::log_handler(const std::string& level, const std::string& message) {
    std::string color;
    if (level == "INFO") {
        color = GREEN;
    } else if (level == "ERROR") {
        color = RED;
    } else if (level == "DEBUG") {
        color = YELLOW;
    } else if (level == "LOG") {
        color = GRAY;
    }
    else {
        color = RESET;
    }
    std::cout << color << "[" << get_current_time() << "] [" << level << "] " << message << RESET << std::endl;
}

std::string zappy::utils::Logger::get_current_time() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}