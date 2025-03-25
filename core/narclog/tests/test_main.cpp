//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NarcLog.h"

class LogTests : public ::testing::Test
{
protected:
    LogTests()
    {
        narclog::createLogger();
    };

    ~LogTests() override
    {
        narclog::destroyLogger();
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST(LoggerInitialisation, LoggerCreation)
{
    EXPECT_NO_THROW(narclog::createLogger(););
    EXPECT_NO_THROW(narclog::destroyLogger(););
}

TEST_F(LogTests, Log_Infos)
{
    testing::internal::CaptureStdout();
    NARCLOG_INFO("MESSAGE");
    EXPECT_THAT(testing::internal::GetCapturedStdout(), ::testing::HasSubstr("MESSAGE"));
}

TEST_F(LogTests, Log_Warning)
{
    testing::internal::CaptureStdout();
    NARCLOG_WARNING("MESSAGE");
    EXPECT_THAT(testing::internal::GetCapturedStdout(), ::testing::HasSubstr("MESSAGE"));
}

TEST_F(LogTests, Log_Error)
{
    EXPECT_THROW(NARCLOG_ERROR("MESSAGE");, narclog::ErrorException);
}

TEST_F(LogTests, Log_Fatal)
{
    EXPECT_THROW(NARCLOG_FATAL("MESSAGE");, narclog::FatalException);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}