#include <gtest/gtest.h>
#include "stl_set.h"

class BaseSuite : public ::testing::Test {
  protected:
    void SetUp();
    void TearDown();
};

TEST_F(BaseSuite, passed) {
    EXPECT_EQ(42, 42);
}