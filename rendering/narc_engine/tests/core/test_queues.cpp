#include "test_includes.h"

INITIALISATION_TESTS(GraphicsQueue_NoThrow)
{
    EngineBuilder builder;
    builder.setValidationLayers(&g_validationLayers);
    EngineInstance instance(&builder);

    MockEngine engine;
    Window windows(&instance, &engine);
    EngineDebugLogger debugLogger(&instance);
    builder.m_debugLogger = &debugLogger;
    builder.m_surface = &windows;
    builder.m_instance = &instance;

    builder.setDeviceExtensions(&g_deviceExtensions);
    DeviceHandler deviceHandler(&builder);

    EXPECT_NO_THROW(GraphicsQueue graphicsQueue(&builder););
}

INITIALISATION_TESTS(PresentQueue_NoThrow)
{
    EngineBuilder builder;
    builder.setValidationLayers(&g_validationLayers);
    EngineInstance instance(&builder);

    MockEngine engine;
    Window windows(&instance, &engine);
    EngineDebugLogger debugLogger(&instance);
    builder.m_debugLogger = &debugLogger;
    builder.m_surface = &windows;
    builder.m_instance = &instance;

    builder.setDeviceExtensions(&g_deviceExtensions);
    DeviceHandler deviceHandler(&builder);

    EXPECT_NO_THROW(PresentQueue presentQueue(&builder););
}