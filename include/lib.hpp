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
#include <boost/program_options.hpp>


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

#define TEAM(message) zappy::utils::Logger::team(message, id)
#define LOG(message) zappy::utils::Logger::log(message)
#define DEBUG(message) zappy::utils::Logger::debug(message)
#define INFO(message) zappy::utils::Logger::info(message)
#define ERROR(message) zappy::utils::Logger::error(message)

// Version information
#define VERSION "0.0.1"

