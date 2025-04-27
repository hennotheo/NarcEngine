#include "test_includes.h"

INITIALISATION_TESTS(Window_NoThrow)
{
    EngineBuilder builder;
    builder.setValidationLayers(&g_validationLayers);
    EngineInstance instance(&builder);

    class MockEngine : public IEngineCallbacks
    {
    public:
        void stop() override {}
    };

    MockEngine engine;

    EXPECT_NO_THROW(Window windows(&instance, &engine););
}

RUNTIME_TESTS(Window_PollEvent_NoThrow)
{
    EngineBuilder builder;
    builder.setValidationLayers(&g_validationLayers);
    EngineInstance instance(&builder);

    class MockEngine : public IEngineCallbacks
    {
    public:
        void stop() override {}
    };

    MockEngine engine;
    Window windows(&instance, &engine);

    EXPECT_NO_THROW(windows.pollEvents(););
}