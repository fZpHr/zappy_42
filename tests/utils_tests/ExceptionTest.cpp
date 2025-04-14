#include <gtest/gtest.h>
#include "../../include/lib.hpp"

TEST(ExceptionTest, ThrowAndCatch) {
   EXPECT_THROW({
       throw ZappyException("Test error");
   }, ZappyException);
   
   try {
       throw ZappyException("Test message");
   } catch (const ZappyException& e) {
       EXPECT_STREQ(e.what(), "Test message");
   }
}