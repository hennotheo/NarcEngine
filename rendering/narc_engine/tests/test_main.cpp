//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "interfaces/IEngine.h"

TEST(NarcEngineTest, EngineCreation)
{
    const narc_engine::EnginePtr engine(narc_engine::createEngine());

    EXPECT_NE(engine, nullptr);
}

TEST(NarcEngineTest, EngineInitShutdown)
{
    const narc_engine::EnginePtr engine(narc_engine::createEngine());

    EXPECT_NO_THROW(engine->init()) << "Engine initialization failed";
    EXPECT_NO_THROW(engine->shutdown()) << "Device shutdown failed";
}

class EngineIntegrationTests : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_engine = narc_engine::createEngine();

        EXPECT_NO_THROW(m_engine->init()) << "Engine initialization failed";
    }

    void TearDown() override
    {
        EXPECT_NO_THROW(m_engine->shutdown()) << "Device shutdown failed";
    }

protected:
    narc_engine::EnginePtr m_engine;
};

TEST_F(EngineIntegrationTests, Engine_ShouldNot_BeMarkedForClosing)
{
    EXPECT_FALSE(m_engine->shouldClose()) << "Engine should not be marked for closing";
}

TEST_F(EngineIntegrationTests, Engine_PollEvents_ShouldNot_Throw)
{
    EXPECT_NO_THROW(m_engine->pollEvents()) << "Engine pollEvents should not throw";
}

TEST_F(EngineIntegrationTests, Engine_Render_ShouldNot_Throw)
{
    EXPECT_NO_THROW(m_engine->render()) << "Engine pollEvents should not throw";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
