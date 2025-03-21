//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "core/DeviceComponent.h"
#include "core/DeviceHandler.h"
#include "core/EngineBinder.h"
#include "core/EngineDebugLogger.h"
#include "core/EngineInstance.h"
#include "core/Window.h"
#include "interfaces/IEngine.h"

using namespace narc_engine;

Window* g_window = nullptr;
EngineInstance* g_instance = nullptr;
EngineDebugLogger* g_logger = nullptr;
DeviceHandler* g_deviceHandler = nullptr;

class MockDeviceComponent : public DeviceComponent
{
public:
    MOCK_METHOD(const DeviceHandler*, getDeviceHandler, (), (const));
};

TEST(NarcEngineTest, WindowCreation)
{
    g_window = new Window();
    EXPECT_NE(nullptr, g_window);
}

TEST(NarcEngineTest, InstanceCreation)
{
    g_instance = new EngineInstance();
    EXPECT_NE(nullptr, g_instance);
}

TEST(NarcEngineTest, WindowInitialization)
{
    EXPECT_NO_THROW(g_window->init(g_instance););
}

TEST(NarcEngineTest, LoggerInitialization)
{
    g_logger = new EngineDebugLogger(g_instance);
    EXPECT_NE(nullptr, g_logger);
}

TEST(NarcEngineTest, DeviceHandlerInitialization)
{
    g_deviceHandler = new DeviceHandler(g_window, g_instance, g_logger);
    EXPECT_NE(nullptr, g_deviceHandler);
}

TEST(NarcEngineTest, EngineInitialization)
{
    IEngine* engine = createEngine();
    EXPECT_NE(nullptr, engine);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
