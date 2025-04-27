#include "test_includes.h"

#include "core/EngineBuilder.h"
#include "core/EngineInstance.h"

using namespace narc_engine;

const std::vector<const char*> g_validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};

INITIALISATION_TESTS(EngineBuilder_ValidationLayers_NoThrow)
{
    EXPECT_NO_THROW(EngineBuilder builder;
        builder.setValidationLayers(&g_validationLayers););
}

INITIALISATION_TESTS(EngineInstance_NoThrow)
{
    EngineBuilder builder;
    builder.setValidationLayers(&g_validationLayers); // No validation layers for this test

    EXPECT_NO_THROW(EngineInstance instance(&builder););
}