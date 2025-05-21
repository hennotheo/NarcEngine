//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

TEST(NarcEngineTest, TestName)
{
    // Your test code here
    EXPECT_TRUE(true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}