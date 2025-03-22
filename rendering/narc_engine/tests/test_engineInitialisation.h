//
// Created by theoh on 3/21/2025.
//

#pragma once

using namespace narc_engine;

class NarcEngineInitialisationTest : public ::testing::Test
{
protected:
    static Window* s_window;
    static EngineInstance* s_instance;
    static EngineDebugLogger* s_logger;
    static DeviceHandler* s_deviceHandler;

    NarcEngineInitialisationTest()
    {
    }

    ~NarcEngineInitialisationTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

Window* NarcEngineInitialisationTest::s_window = nullptr;
EngineInstance* NarcEngineInitialisationTest::s_instance = nullptr;
EngineDebugLogger* NarcEngineInitialisationTest::s_logger = nullptr;
DeviceHandler* NarcEngineInitialisationTest::s_deviceHandler = nullptr;

TEST_F(NarcEngineInitialisationTest, WindowCreation)
{
    s_window = new Window();
    EXPECT_NE(nullptr, s_window);
}

TEST_F(NarcEngineInitialisationTest, InstanceCreation)
{
    s_instance = new EngineInstance();
    EXPECT_NE(nullptr, s_instance);
}

TEST_F(NarcEngineInitialisationTest, WindowInitialization)
{
    EXPECT_NO_THROW(s_window->init(s_instance););
}

TEST_F(NarcEngineInitialisationTest, LoggerInitialization)
{
    s_logger = new EngineDebugLogger(s_instance);
    EXPECT_NE(nullptr, s_logger);
}

TEST_F(NarcEngineInitialisationTest, DeviceHandlerInitialization)
{
    s_deviceHandler = new DeviceHandler(s_window, s_instance, s_logger);
    EXPECT_NE(nullptr, s_deviceHandler);
}

TEST_F(NarcEngineInitialisationTest, EngineInitialization)
{
    IEngine* engine = createEngine();
    EXPECT_NE(nullptr, engine);
}
