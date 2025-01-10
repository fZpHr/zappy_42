#pragma once

#ifdef TESTS_ENABLED
    #include <boost/test/unit_test.hpp>
#endif

using namespace std;

#include "../include/utils/Logger.hpp"
#include "../include/utils/Exception.hpp"

#define TEAM(message) Logger::team(message, id)
#define LOG(message) Logger::log(message)
#define DEBUG(message) Logger::debug(message)
#define INFO(message) Logger::info(message)
#define ERROR(message) Logger::error(message)

#define VERSION "0.0.1"

