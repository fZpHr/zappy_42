#include <gtest/gtest.h>
#include "../../include/lib.hpp"

TEST(LoggerTest, LogLevels) {
   EXPECT_NO_THROW({
     Logger::info("Info message");
     Logger::error("Error message");
     Logger::debug("Debug message");
   });
}