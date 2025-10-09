//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "NarcLog.h"

TEST(LOGGING_TESTS, ERROR_THROW_SPECIFIC_TYPES) {
    EXPECT_THROW({ NARC_ERROR_LOGIC("Logic debug message"); }, narc_log::LogicError)
        << "NARC_ERROR_LOGIC should throw narc_log::LogicError";
    EXPECT_THROW({ NARC_ERROR_RUNTIME("Runtime debug message"); }, narc_log::RuntimeError)
        << "NARC_ERROR_RUNTIME should throw narc_log::RuntimeError";
}

TEST(LOGGING_TESTS, ERROR_THROW_MESSAGE_CONTENT) {
    try {
        NARC_ERROR_LOGIC("Logic debug message");
        FAIL() << "Expected LogicError to be thrown";
    } catch (const narc_log::LogicError &e) {
        EXPECT_STREQ(e.what(), std::string("Logic debug message").c_str());
    } catch (...) {
        FAIL() << "Caught wrong exception type for LogicError";
    }

    try {
        NARC_ERROR_RUNTIME("Runtime debug message");
        FAIL() << "Expected RuntimeError to be thrown";
    } catch (const narc_log::RuntimeError &e) {
        EXPECT_STREQ(e.what(), std::string("Runtime debug message").c_str());
    } catch (...) {
        FAIL() << "Caught wrong exception type for RuntimeError";
    }
}

TEST(LOGGING_TESTS, LOG_NO_THROW) {
    EXPECT_NO_THROW(NARC_LOG_DEBUG("Debug"));
    EXPECT_NO_THROW(NARC_LOG_INFO("Info"));
    EXPECT_NO_THROW(NARC_LOG_WARNING("Warning"));
    EXPECT_NO_THROW(NARC_LOG_ERROR("Error"));
    EXPECT_NO_THROW(NARC_LOG_FATAL("Fatal"));
}

TEST(LOGGING_TESTS, LOG_FORMATTING_NO_THROW) {
    EXPECT_NO_THROW(NARC_LOG_INFO("Value: {}", 42));
    EXPECT_NO_THROW(NARC_LOG_WARNING("Pair: {} - {}", "left", "right"));
}

TEST(LOGGING_TESTS, INIT_SIGNAL_HANDLING_NO_THROW) {
    EXPECT_NO_THROW(narc_log::init_signal_handling());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
