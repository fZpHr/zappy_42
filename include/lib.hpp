#pragma once

// Standard libraries
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string_view>
#include <poll.h>
#include <iomanip>
#include <set>

// Boost libraries
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

// Testing framework
#ifdef TESTS_ENABLED
    #include <boost/test/unit_test.hpp>
#endif

#include "utils/Logger.hpp"
#include "utils/Exception.hpp"
#include "network/SocketHandler.hpp"
#include "network/NetworkManager.hpp"
#include "core/Client.hpp"
#include "core/Server.hpp"

using namespace std;

namespace zappy {

    namespace {
        constexpr int DEFAULT_PORT = 4242;
        constexpr int BUFFER_SIZE = 1024;
        constexpr int MAX_CLIENTS = 100;
    }
    
    namespace {
        constexpr int SUCCESS = 0;
        constexpr int CONNECTION_ERROR = -1;
        constexpr int INVALID_MESSAGE = -2;
    }

    namespace {
        const std::string RESET = "\033[0m";
        const std::string RED = "\033[31m";
        const std::string GRAY = "\033[90m";
        const std::string YELLOW = "\033[33m";
        const std::string GREEN = "\033[32m";
    }
}

#define ZAPPY_LOG(message) zappy::utils::Logger::log(message)
#define ZAPPY_DEBUG(message) zappy::utils::Logger::debug(message)
#define ZAPPY_INFO(message) zappy::utils::Logger::info(message)
#define ZAPPY_ERROR(message) zappy::utils::Logger::error(message)

// Version information
#define ZAPPY_VERSION "0.0.1"

