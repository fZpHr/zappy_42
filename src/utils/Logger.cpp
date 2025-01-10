#include "../../include/utils/Logger.hpp"

const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";
const std::string GRAY = "\033[90m";
const std::string LIGHT_GRAY = "\033[37m";
const std::string DARK_RED = "\033[31m";
const std::string LIGHT_RED = "\033[91m";
const std::string DARK_GREEN = "\033[32m";
const std::string LIGHT_GREEN = "\033[92m";
const std::string DARK_YELLOW = "\033[33m";
const std::string LIGHT_YELLOW = "\033[93m";
const std::string DARK_BLUE = "\033[34m";
const std::string LIGHT_BLUE = "\033[94m";
const std::string DARK_MAGENTA = "\033[35m";
const std::string LIGHT_MAGENTA = "\033[95m";
const std::string DARK_CYAN = "\033[36m";
const std::string LIGHT_CYAN = "\033[96m";

std::unordered_map<size_t, std::string> Logger::id_color_map;
std::vector<std::string> Logger::colors = {
    BLUE, MAGENTA, CYAN, WHITE, GRAY,
    LIGHT_GRAY, DARK_RED, LIGHT_RED, DARK_GREEN, LIGHT_GREEN,
    DARK_YELLOW, LIGHT_YELLOW, DARK_BLUE, LIGHT_BLUE,
    DARK_MAGENTA, LIGHT_MAGENTA, DARK_CYAN, LIGHT_CYAN
};
size_t Logger::color_index = 0;

void Logger::team(const std::string &message, const size_t &id) {
    log_handler("TEAM " +  std::to_string(id), message, id);
}

void Logger::info(const std::string &message, const size_t &id) {
    log_handler("INFO", message, id);
}

void Logger::error(const std::string &message, const size_t &id) {
    log_handler("ERROR", message, id);
}

void Logger::debug(const std::string &message, const size_t &id) {
    log_handler("DEBUG", message, id);
}

void Logger::log(const std::string &message, const size_t &id) {
    log_handler("LOG", message, id);
}

void Logger::log_handler(const std::string& level, const std::string& message, const size_t &id) {
     std::string color;
    bool error = false;
    if (id == 0) {
        if (level == "INFO") {
            color = GREEN;
        } else if (level == "ERROR") {
            color = RED;
            error = true;
        } else if (level == "DEBUG") {
            color = YELLOW;
        } else if (level == "LOG") {
            color = GRAY;
        } else {
            color = RESET;
        }
    } else {
        if (id_color_map.find(id) == id_color_map.end()) {
            id_color_map[id] = colors[color_index];
            color_index = (color_index + 1) % colors.size();
        }
        color = id_color_map[id];
    }
    if (error) {
        std::cerr << color << "[" << get_current_time() << "] [" << level << "] " << message << RESET << std::endl;
    } else {
        std::cout << color << "[" << get_current_time() << "] [" << level << "] " << message << RESET << std::endl;
    }
}

std::string Logger::get_current_time() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &now_tm);
    return buf;
}
