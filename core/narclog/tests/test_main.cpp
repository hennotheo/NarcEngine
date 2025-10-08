//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NarcLog.h"


TEST(LOGGING_TEST, INFO) {
    EXPECT_NO_THROW(
        NARC_LOG_DEBUG("Debug");
    );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
