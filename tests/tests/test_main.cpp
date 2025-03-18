//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

TEST(ExampleTest, SimpleTest) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}