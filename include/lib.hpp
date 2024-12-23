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

// Boost libraries
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

// Testing framework
#ifdef TESTS_ENABLED
    #include <boost/test/unit_test.hpp>
#endif

// Project headers
#include "utils/Logger.hpp"
#include "utils/Exception.hpp"
#include "network/SocketHandler.hpp"
#include "network/NetworkManager.hpp"
#include "core/Client.hpp"
#include "core/Server.hpp"

// Namespaces aliases for convenience
namespace asio = boost::asio;
namespace sys = boost::system;

// Project namespace
namespace zappy {
    // Common types
    using tcp = boost::asio::ip::tcp;
    using error_code = sys::error_code;
    
    // Smart pointer aliases
    template<typename T>
    using Ptr = std::shared_ptr<T>;
    
    template<typename T>
    using UPtr = std::unique_ptr<T>;
    
    // String aliases
    using String = std::string;
    // using StringView = std::string_view;
    
    // Global constants
    namespace constants {
        constexpr int DEFAULT_PORT = 4242;
        constexpr int BUFFER_SIZE = 1024;
        constexpr int MAX_CLIENTS = 100;
    }
    
    // Error codes
    namespace error {
        constexpr int SUCCESS = 0;
        constexpr int CONNECTION_ERROR = -1;
        constexpr int INVALID_MESSAGE = -2;
    }
}

// Utility macros
#define ZAPPY_LOG(level, message) zappy::utils::Logger::level(message)
#define ZAPPY_DEBUG(message) ZAPPY_LOG(debug, message)
#define ZAPPY_INFO(message) ZAPPY_LOG(info, message)
#define ZAPPY_ERROR(message) ZAPPY_LOG(error, message)

// Version information
#define ZAPPY_VERSION "0.0.1"