#include "test_includes.h"

INITIALISATION_TESTS(PhysicalDevice_NoThrow)
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

    EXPECT_NO_THROW(PhysicalDevice physicalDevice(&builder););
}

INITIALISATION_TESTS(Device_NoThrow)
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
    PhysicalDevice physicalDevice(&builder);

    EXPECT_NO_THROW(LogicalDevice logicalDevice(&builder, &physicalDevice););
}

INITIALISATION_TESTS(DeviceHandler_NoThrow)
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

    EXPECT_NO_THROW(DeviceHandler deviceHandler(&builder););
}