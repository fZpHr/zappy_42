#include <gtest/gtest.h>
#include "../../include/lib.hpp"

TEST(ExceptionTest, ThrowAndCatch) {
   EXPECT_THROW({
       throw zappy::utils::ZappyException("Test error");
   }, zappy::utils::ZappyException);
   
   try {
       throw zappy::utils::ZappyException("Test message");
   } catch (const zappy::utils::ZappyException& e) {
       EXPECT_STREQ(e.what(), "Test message");
   }
}