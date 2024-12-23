#include <gtest/gtest.h>
#include "../../include/lib.hpp"

TEST(LoggerTest, LogLevels) {
   EXPECT_NO_THROW({
       zappy::utils::Logger::info("Info message");
       zappy::utils::Logger::error("Error message");
       zappy::utils::Logger::debug("Debug message");
   });
}