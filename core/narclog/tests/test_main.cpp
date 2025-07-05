//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NarcLog.h"

#define INITIALISATION_TESTS(testName) TEST(NarcLog_Initialisation, testName)
#define LOG_TESTS(testName) TEST(NarcLog_Logging, testName)

class LogTests
{
public:
    LogTests()
    {
        narclog::createLogger();
    };

    ~LogTests()
    {
        narclog::destroyLogger();
    }
};

INITIALISATION_TESTS(LoggerCreation_NoThrow)
{
    EXPECT_NO_THROW(LogTests log{};);
}

INITIALISATION_TESTS(LoggerDestroy_NoThrow)
{
    narclog::createLogger();
    EXPECT_NO_THROW(narclog::destroyLogger(););
}

// LOG_TESTS(Log_Infos)
// {
//     LogTests logger;
//
//     testing::internal::CaptureStdout();
//     NARCLOG_INFO("MESSAGE");
//     std::string output = testing::internal::GetCapturedStdout();
//     EXPECT_THAT(output, ::testing::HasSubstr("MESSAGE"));
// }
//
// LOG_TESTS(Log_Warning)
// {
//     LogTests logger;
//
//     testing::internal::CaptureStdout();
//     NARCLOG_WARNING("MESSAGE");
//     std::string output = testing::internal::GetCapturedStdout();
//     EXPECT_THAT(output, ::testing::HasSubstr("MESSAGE"));
// }

LOG_TESTS(Log_Error)
{
    LogTests logger;

    EXPECT_THROW(NARCLOG_ERROR("MESSAGE");, narclog::ErrorException);
}

LOG_TESTS(Log_Fatal)
{
    LogTests logger;
    EXPECT_THROW(NARCLOG_FATAL("MESSAGE");, narclog::FatalException);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}