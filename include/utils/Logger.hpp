#pragma once
#include "../lib.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace zappy {
    namespace utils {

        class Logger {
        public:
            static void info(const std::string &message, const size_t &id = 0);
            static void error(const std::string &message, const size_t &id = 0);
            static void debug(const std::string &message, const size_t &id = 0);
            static void log(const std::string &message, const size_t &id = 0);
            static void team(const std::string &message, const size_t &id);

        private:
            static void log_handler(const std::string& level, const std::string& message, const size_t &id);
            static std::string get_current_time();
            static std::unordered_map<size_t, std::string> id_color_map;
            static std::vector<std::string> colors;
            static size_t color_index;
        };

    }
}