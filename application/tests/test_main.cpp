//
// Created by theoh on 3/23/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "Application.h"

TEST(NarcEngineApp, ApplicationCreation)
{
    EXPECT_NO_THROW(narc::Application app;);
};

TEST(NarcEngineApp, ApplicationStart)
{
    narc::Application app;
    EXPECT_NO_THROW(app.start());
    app.stop();
};

TEST(NarcEngineApp, ApplicationLoopBody)
{
    narc::Application app;
    app.start();
    EXPECT_NO_THROW(app.appLoopBody());
    app.stop();
};

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
